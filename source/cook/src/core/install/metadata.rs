use alloc::{string::ToString, format};

/**
    cook - E-comOS Packages Manager
    Copyright (C) 2025,2026  E-comOS User Mode Team cook Group & Saladin5101

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
use log::{info, error};

/* Placeholder modules for no_std compatibility
use core::path::Path;
use core::fs;
*/

pub struct PackageMetadata {
    pub name: String,
    pub version: String,
    pub release_date: String,
    pub build_date: String,
    pub publisher: String,
    pub verified: String,
    pub author: String,
    pub run_on: String,
    pub description: String,
    pub license: String,
    pub use_packages: String,
    pub dependencies: String,
    pub contact: String,
    pub build: Option<BuildOptions>,
    pub checksum: Option<String>,
    pub tests: Option<TestOptions>,
}

pub struct BuildOptions {
    pub compiler: String,
    pub flags: Vec<String>,
    pub output: String,
}

pub struct TestOptions {
    pub command: String,
}

pub enum MetadataError {
    EmptyField,
    InvalidVersion,
    InvalidStatus,
    InvalidDependency,
    InvalidPlatform,
}

impl PackageMetadata {
    pub fn validate(&self) -> Result<(), MetadataError> {
        // Check required fields
        if self.name.is_empty() || self.version.is_empty() || 
           self.publisher.is_empty() || self.author.is_empty() ||
           self.description.is_empty() || self.license.is_empty() {
            return Err(MetadataError::EmptyField);
        }
        
        // Check version format
        if !self.version.contains('.') {
            return Err(MetadataError::InvalidVersion);
        }
        
        // Check verification status
        let valid_statuses = ["yes", "true", "verified", "no", "false", "unverified"];
        if !valid_statuses.contains(&self.verified.as_str()) {
            return Err(MetadataError::InvalidStatus);
        }
        
        // Check dependency format
        if self.dependencies != "No" && !self.dependencies.starts_with("cook://") {
            return Err(MetadataError::InvalidDependency);
        }
        
        if self.use_packages != "NOTHING" && !self.use_packages.starts_with("cook://") {
            return Err(MetadataError::InvalidDependency);
        }
        
        // Check platform support
        let valid_platforms = ["x86", "x64", "ARM", "ARM64", "RISC-V"];
        for platform in self.supported_platforms() {
            if !valid_platforms.contains(&platform.as_str()) {
                return Err(MetadataError::InvalidPlatform);
            }
        }
        
        // Validate build options
        if let Some(build) = &self.build {
            if build.compiler.is_empty() || build.output.is_empty() {
                return Err(MetadataError::EmptyField);
            }
        }

        // Validate checksum
        if let Some(checksum) = &self.checksum {
            if checksum.len() != 64 { // Example: SHA-256 length
                return Err(MetadataError::InvalidDependency);
            }
        }

        // Validate tests
        if let Some(tests) = &self.tests {
            if tests.command.is_empty() {
                return Err(MetadataError::EmptyField);
            }
        }

        Ok(())
    }
    
    pub fn is_verified(&self) -> bool {
        self.verified == "yes" || self.verified == "true" || self.verified == "verified"
    }
    
    pub fn supported_platforms(&self) -> Vec<String> {
        let mut platforms = Vec::new();
        let mut current = String::new();
        
        for ch in self.run_on.chars() {
            if ch == ',' {
                if !current.is_empty() {
                    platforms.push(current.trim().to_string());
                    current = String::new();
                }
            } else {
                current.push(ch);
            }
        }
        
        if !current.is_empty() {
            platforms.push(current.trim().to_string());
        }
        
        platforms
    }
    
    pub fn has_dependencies(&self) -> bool {
        self.dependencies != "No"
    }
    
    pub fn dependency_list(&self) -> Vec<String> {
        if !self.has_dependencies() {
            return Vec::new();
        }
        
        let mut deps = Vec::new();
        let mut current = String::new();
        
        for ch in self.dependencies.chars() {
            if ch == ',' {
                if !current.is_empty() {
                    deps.push(current.trim().to_string());
                    current = String::new();
                }
            } else {
                current.push(ch);
            }
        }
        
        if !current.is_empty() {
            deps.push(current.trim().to_string());
        }
        
        deps
    }

    /// Resolves dependencies recursively and validates their availability.
    pub fn resolve_dependencies(&self, available_packages: &Vec<String>) -> Result<Vec<String>, MetadataError> {
        let mut resolved = Vec::new();
        let dependencies = self.dependency_list();

        for dep in dependencies {
            if available_packages.contains(&dep) {
                resolved.push(dep);
            } else {
                return Err(MetadataError::InvalidDependency);
            }
        }

        Ok(resolved)
    }

    /// Ensures all dependencies are installed or compiled before the main package.
    pub fn ensure_dependencies_installed(&self, installed_packages: &Vec<String>, compile_dependency: fn(&str) -> Result<(), String>) -> Result<(), MetadataError> {
        let dependencies = self.dependency_list();

        for dep in dependencies {
            if !installed_packages.contains(&dep) {
                // Attempt to compile the dependency
                if let Err(err) = compile_dependency(&dep) {
                    error!("Failed to compile dependency {}: {}", dep, err);
                    return Err(MetadataError::InvalidDependency);
                }
            }
        }

        Ok(())
    }

    /// Checks if the package is already compiled and cached.
    pub fn is_cached(&self, cache_dir: &str) -> bool {
        let _cache_path = format!("{}/{}-{}.bin", cache_dir, self.name, self.version);
        /* Placeholder for no_std compatibility
        Path::new(&cache_path).exists()
        */
        false // TODO: Implement actual cache check
    }

    /// Verifies the checksum of the package source.
    pub fn verify_checksum(&self, source_path: &str) -> Result<(), MetadataError> {
        if let Some(expected_checksum) = &self.checksum {
            use sha2::{Digest, Sha256};
            use alloc::vec::Vec;

            /*
            let data = fs::read(source_path).map_err(|_| MetadataError::InvalidDependency)?;
            */
            let data = Vec::new(); // TODO: Replace with actual file reading
            let mut hasher = Sha256::new();
            hasher.update(data);
            let calculated_checksum = format!("{:x}", hasher.finalize());

            if &calculated_checksum != expected_checksum {
                return Err(MetadataError::InvalidDependency);
            }
        }

        Ok(())
    }

    /// Performs incremental compilation by checking for changes.
    pub fn compile_incrementally(&self, source_dir: &str, cache_dir: &str, compile: fn(&str, &str) -> Result<(), String>) -> Result<(), MetadataError> {
        if self.is_cached(cache_dir) {
            info!("Package {} is already cached.", self.name);
            return Ok(());
        }

        let _source_path = format!("{}/{}", source_dir, self.name);
        self.verify_checksum(&_source_path)?;

        compile(&_source_path, cache_dir).map_err(|_| MetadataError::InvalidDependency)
    }
}