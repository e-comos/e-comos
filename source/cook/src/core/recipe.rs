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

pub struct Recipe {
    pub name: String,
    pub version: String,
    pub description: String,
    pub repository: Repository,
    pub build: BuildConfig,
    pub dependencies: Vec<String>,
    pub prerequisites: Vec<String>,
}

pub struct Repository {
    pub url: String,
    pub branch: String,
    pub commit: Option<String>,
}

pub struct BuildConfig {
    pub build_system: String,
    pub configure_args: Vec<String>,
    pub build_args: Vec<String>,
    pub install_args: Vec<String>,
    pub environment: Vec<EnvVar>,
}

pub struct EnvVar {
    pub key: String,
    pub value: String,
}

pub enum RecipeError {
    ParseError,
    InvalidFormat,
    MissingField,
    InvalidBuildSystem,
}

impl Recipe {
    pub fn parse_json(content: &str) -> Result<Self, RecipeError> {
        let mut recipe = Recipe {
            name: String::new(),
            version: String::new(),
            description: String::new(),
            repository: Repository {
                url: String::new(),
                branch: String::from("main"),
                commit: None,
            },
            build: BuildConfig {
                build_system: String::new(),
                configure_args: Vec::new(),
                build_args: Vec::new(),
                install_args: Vec::new(),
                environment: Vec::new(),
            },
            dependencies: Vec::new(),
            prerequisites: Vec::new(),
        };
        
        // Simple JSON parsing
        let lines: Vec<&str> = content.lines().collect();
        let mut in_repository = false;
        let mut in_build = false;
        let mut in_dependencies = false;
        let mut in_prerequisites = false;
        
        for line in lines {
            let line = line.trim();
            
            if line.contains("\"repository\"") {
                in_repository = true;
                continue;
            }
            if line.contains("\"build\"") {
                in_build = true;
                continue;
            }
            if line.contains("\"dependencies\"") {
                in_dependencies = true;
                continue;
            }
            if line.contains("\"prerequisites\"") {
                in_prerequisites = true;
                continue;
            }
            
            if line == "}" {
                in_repository = false;
                in_build = false;
                in_dependencies = false;
                in_prerequisites = false;
                continue;
            }
            
            if in_repository {
                if let Some(value) = Self::extract_json_value(line, "url") {
                    recipe.repository.url = value;
                } else if let Some(value) = Self::extract_json_value(line, "branch") {
                    recipe.repository.branch = value;
                } else if let Some(value) = Self::extract_json_value(line, "commit") {
                    recipe.repository.commit = Some(value);
                }
            } else if in_build {
                if let Some(value) = Self::extract_json_value(line, "build_system") {
                    recipe.build.build_system = value;
                }
            } else if in_dependencies {
                if let Some(dep) = Self::extract_array_item(line) {
                    recipe.dependencies.push(dep);
                }
            } else if in_prerequisites {
                if let Some(prereq) = Self::extract_array_item(line) {
                    recipe.prerequisites.push(prereq);
                }
            } else {
                if let Some(value) = Self::extract_json_value(line, "name") {
                    recipe.name = value;
                } else if let Some(value) = Self::extract_json_value(line, "version") {
                    recipe.version = value;
                } else if let Some(value) = Self::extract_json_value(line, "description") {
                    recipe.description = value;
                }
            }
        }
        
        Ok(recipe)
    }
    
    fn extract_json_value(line: &str, key: &str) -> Option<String> {
        if line.contains(key) {
            if let Some(colon_pos) = line.find(':') {
                let value_part = &line[colon_pos + 1..];
                let cleaned = value_part.trim().trim_matches('"').trim_matches(',');
                return Some(String::from(cleaned));
            }
        }
        None
    }
    
    fn extract_array_item(line: &str) -> Option<String> {
        let trimmed = line.trim();
        if trimmed.starts_with('"') && trimmed.contains('"') {
            let cleaned = trimmed.trim_matches('"').trim_matches(',');
            return Some(String::from(cleaned));
        }
        None
    }
    
    pub fn validate(&self) -> Result<(), RecipeError> {
        if self.name.is_empty() || self.version.is_empty() || self.repository.url.is_empty() {
            return Err(RecipeError::MissingField);
        }
        
        let valid_build_systems = ["make", "cmake", "ecomos-cc", "cargo", "npm", "gradle", "maven"];
        if !valid_build_systems.contains(&self.build.build_system.as_str()) {
            return Err(RecipeError::InvalidBuildSystem);
        }
        
        Ok(())
    }
}