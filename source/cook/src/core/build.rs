/**
    Cook - E-comOS Packages Manager
    Copyright (C) 2025  E-comOS User Mode Team Cook Group & Saladin5101

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

extern crate alloc;
use alloc::{string::String, vec::Vec};
use super::recipe::{Recipe, Repository, BuildConfig};

pub enum BuildError {
    CloneFailed,
    ConfigureFailed,
    CompileFailed,
    InstallFailed,
    PrerequisitesMissing,
    InvalidBuildSystem,
}

pub struct Builder {
    pub work_dir: String,
    pub install_prefix: String,
}

impl Builder {
    pub fn new(work_dir: String, install_prefix: String) -> Self {
        Self { work_dir, install_prefix }
    }
    
    pub fn build_from_recipe(&self, recipe: &Recipe) -> Result<String, BuildError> {
        // 1. Check prerequisites
        self.check_prerequisites(&recipe.prerequisites)?;
        
        // 2. Clone source code
        let source_dir = self.clone_repository(&recipe.repository)?;
        
        // 3. Configure build
        self.configure_build(&recipe.build, &source_dir)?;
        
        // 4. Compile
        self.compile(&recipe.build, &source_dir)?;
        
        // 5. Install
        let install_path = self.install(&recipe.build, &source_dir)?;
        
        Ok(install_path)
    }
    
    fn check_prerequisites(&self, prerequisites: &[String]) -> Result<(), BuildError> {
        for prereq in prerequisites {
            if !self.is_tool_available(prereq) {
                return Err(BuildError::PrerequisitesMissing);
            }
        }
        Ok(())
    }
    
    fn clone_repository(&self, repo: &Repository) -> Result<String, BuildError> {
        let mut clone_dir = String::new();
        clone_dir.push_str(&self.work_dir);
        clone_dir.push_str("/source");
        
        // Build git clone command
        let mut git_cmd = String::new();
        git_cmd.push_str("git clone ");
        if !repo.branch.is_empty() && repo.branch != "main" {
            git_cmd.push_str("-b ");
            git_cmd.push_str(&repo.branch);
            git_cmd.push(' ');
        }
        git_cmd.push_str(&repo.url);
        git_cmd.push(' ');
        git_cmd.push_str(&clone_dir);
        
        // Execute git clone
        if !self.execute_command(&git_cmd, &self.work_dir) {
            return Err(BuildError::CloneFailed);
        }
        
        // Checkout specific commit if specified
        if let Some(commit) = &repo.commit {
            let mut checkout_cmd = String::new();
            checkout_cmd.push_str("git checkout ");
            checkout_cmd.push_str(commit);
            
            if !self.execute_command(&checkout_cmd, &clone_dir) {
                return Err(BuildError::CloneFailed);
            }
        }
        
        Ok(clone_dir)
    }
    
    fn configure_build(&self, build_config: &BuildConfig, source_dir: &str) -> Result<(), BuildError> {
        match build_config.build_system.as_str() {
            "make" => self.configure_make(build_config, source_dir),
            "cmake" => self.configure_cmake(build_config, source_dir),
            "ecomos-cc" => self.configure_ecomos(build_config, source_dir),
            "cargo" => Ok(()), // Cargo doesn't need separate configure step
            "npm" => self.configure_npm(build_config, source_dir),
            _ => Err(BuildError::InvalidBuildSystem),
        }
    }
    
    fn compile(&self, build_config: &BuildConfig, source_dir: &str) -> Result<(), BuildError> {
        let compile_cmd = match build_config.build_system.as_str() {
            "make" => "make",
            "cmake" => "make", 
            "ecomos-cc" => self.build_ecomos_compile_cmd(source_dir),
            "cargo" => "cargo build --release",
            "npm" => "npm run build",
            _ => return Err(BuildError::InvalidBuildSystem),
        };
        
        if !self.execute_command(compile_cmd, source_dir) {
            return Err(BuildError::CompileFailed);
        }
        
        Ok(())
    }
    
    fn install(&self, build_config: &BuildConfig, source_dir: &str) -> Result<String, BuildError> {
        let mut install_cmd = String::new();
        
        match build_config.build_system.as_str() {
            "make" | "cmake" => {
                install_cmd.push_str("make install PREFIX=");
                install_cmd.push_str(&self.install_prefix);
            },
            "cargo" => {
                install_cmd.push_str("cargo install --path . --root ");
                install_cmd.push_str(&self.install_prefix);
            },
            "npm" => {
                install_cmd.push_str("npm install -g --prefix ");
                install_cmd.push_str(&self.install_prefix);
            },
            _ => return Err(BuildError::InvalidBuildSystem),
        }
        
        if !self.execute_command(&install_cmd, source_dir) {
            return Err(BuildError::InstallFailed);
        }
        
        Ok(self.install_prefix.clone())
    }
    
    fn configure_make(&self, build_config: &BuildConfig, source_dir: &str) -> Result<(), BuildError> {
        // Check if configure script exists
        let mut configure_path = String::new();
        configure_path.push_str(source_dir);
        configure_path.push_str("/configure");
        
        if self.file_exists(&configure_path) {
            let mut configure_cmd = String::new();
            configure_cmd.push_str("./configure --prefix=");
            configure_cmd.push_str(&self.install_prefix);
            
            for arg in &build_config.configure_args {
                configure_cmd.push(' ');
                configure_cmd.push_str(arg);
            }
            
            if !self.execute_command(&configure_cmd, source_dir) {
                return Err(BuildError::ConfigureFailed);
            }
        }
        
        Ok(())
    }
    
    fn configure_cmake(&self, build_config: &BuildConfig, source_dir: &str) -> Result<(), BuildError> {
        let mut cmake_cmd = String::new();
        cmake_cmd.push_str("cmake -DCMAKE_INSTALL_PREFIX=");
        cmake_cmd.push_str(&self.install_prefix);
        cmake_cmd.push_str(" .");
        
        for arg in &build_config.configure_args {
            cmake_cmd.push(' ');
            cmake_cmd.push_str(arg);
        }
        
        if !self.execute_command(&cmake_cmd, source_dir) {
            return Err(BuildError::ConfigureFailed);
        }
        
        Ok(())
    }
    
    fn configure_npm(&self, _build_config: &BuildConfig, source_dir: &str) -> Result<(), BuildError> {
        if !self.execute_command("npm install", source_dir) {
            return Err(BuildError::ConfigureFailed);
        }
        Ok(())
    }
    
    fn is_tool_available(&self, tool: &str) -> bool {
        let mut which_cmd = String::new();
        which_cmd.push_str("which ");
        which_cmd.push_str(tool);
        
        self.execute_command(&which_cmd, &self.work_dir)
    }
    
    fn execute_command(&self, command: &str, working_dir: &str) -> bool {
        // Platform-specific command execution
        // TODO: Implement actual command execution via system calls
        true // Placeholder
    }
    
    fn file_exists(&self, path: &str) -> bool {
        // Platform-specific file existence check
        // TODO: Implement actual file existence check
        false // Placeholder
    }
    
    fn build_ecomos_compile_cmd(&self, _source_dir: &str) -> &str {
        // Use E-comOS C/C++ Language Compiler
        "ecomos-cc -O2 -o output *.c *.cpp"
    }
    
    fn configure_ecomos(&self, build_config: &BuildConfig, source_dir: &str) -> Result<(), BuildError> {
        // E-comOS compiler configuration
        let mut compile_cmd = String::new();
        compile_cmd.push_str("ecomos-cc");
        
        // Add optimization flags
        compile_cmd.push_str(" -O2");
        
        // Add include directories
        for arg in &build_config.configure_args {
            if arg.starts_with("-I") {
                compile_cmd.push(' ');
                compile_cmd.push_str(arg);
            }
        }
        
        // Set output directory
        compile_cmd.push_str(" -o ");
        compile_cmd.push_str(&self.install_prefix);
        compile_cmd.push_str("/bin/");
        
        // Add source files
        compile_cmd.push_str(" *.c *.cpp *.cc");
        
        if !self.execute_command(&compile_cmd, source_dir) {
            return Err(BuildError::ConfigureFailed);
        }
        
        Ok(())
    }
}