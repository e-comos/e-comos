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
use alloc::{string::String, vec::Vec, format};
use super::recipe::{Recipe, RecipeError};
use super::build::{Builder, BuildError};
use super::install::{Downloader, DownloadError};

pub enum CookError {
    RecipeDownloadFailed(DownloadError),
    RecipeParseFailed(RecipeError),
    BuildFailed(BuildError),
    RepositoryNotFound,
}

pub struct CookManager {
    pub recipe_sources: Vec<String>,
    pub work_dir: String,
    pub install_prefix: String,
    pub downloader: Downloader,
    pub builder: Builder,
}

impl CookManager {
    pub fn new(work_dir: String, install_prefix: String) -> Self {
        let cache_dir = format!("{}/cache", work_dir);
        
        Self {
            recipe_sources: Vec::new(),
            work_dir: work_dir.clone(),
            install_prefix: install_prefix.clone(),
            downloader: Downloader::new(cache_dir),
            builder: Builder::new(work_dir, install_prefix),
        }
    }
    
    pub fn add_recipe_source(&mut self, source_url: String) {
        self.recipe_sources.push(source_url);
    }
    
    pub fn cook_package(&self, package_name: &str) -> Result<(), CookError> {
        // 1. Download recipe from sources
        let recipe_content = self.download_recipe(package_name)?;
        
        // 2. Parse recipe JSON
        let recipe = Recipe::parse_json(&recipe_content)
            .map_err(CookError::RecipeParseFailed)?;
        
        // 3. Validate recipe
        recipe.validate().map_err(CookError::RecipeParseFailed)?;
        
        // 4. Build from source using recipe
        self.builder.build_from_recipe(&recipe)
            .map_err(CookError::BuildFailed)?;
        
        Ok(())
    }
    
    fn download_recipe(&self, package_name: &str) -> Result<String, CookError> {
        for source in &self.recipe_sources {
            let recipe_url = self.build_recipe_url(source, package_name);
            
            // Try to download recipe from this source
            if let Ok(recipe_path) = self.download_recipe_file(&recipe_url) {
                if let Ok(content) = self.read_recipe_file(&recipe_path) {
                    return Ok(content);
                }
            }
        }
        
        Err(CookError::RepositoryNotFound)
    }
    
    fn build_recipe_url(&self, source: &str, package_name: &str) -> String {
        let mut url = String::new();
        url.push_str(source);
        if !source.ends_with('/') {
            url.push('/');
        }
        url.push_str(package_name);
        url.push_str(".json");
        url
    }
    
    fn download_recipe_file(&self, url: &str) -> Result<String, CookError> {
        // Create a fake package for downloader
        let recipe_package = super::install::Package {
            name: String::from("recipe"),
            version: String::from("1.0"),
            url: String::from(url),
            size: 0,
        };
        
        self.downloader.download_package(&recipe_package)
            .map_err(CookError::RecipeDownloadFailed)
    }
    
    fn read_recipe_file(&self, path: &str) -> Result<String, CookError> {
        // Read recipe file content
        if let Ok(data) = self.read_file_bytes(path) {
            let mut content = String::new();
            for byte in data {
                content.push(byte as char);
            }
            Ok(content)
        } else {
            Err(CookError::RepositoryNotFound)
        }
    }
    
    fn read_file_bytes(&self, _path: &str) -> Result<Vec<u8>, CookError> {
        // Platform-specific file reading
        // TODO: Implement actual file reading
        Ok(Vec::new())
    }
    
    pub fn list_available_recipes(&self) -> Vec<String> {
        let mut recipes = Vec::new();
        
        for source in &self.recipe_sources {
            if let Ok(source_recipes) = self.fetch_recipe_list(source) {
                recipes.extend(source_recipes);
            }
        }
        
        recipes
    }
    
    fn fetch_recipe_list(&self, _source: &str) -> Result<Vec<String>, CookError> {
        // Fetch list of available recipes from source
        // TODO: Implement recipe listing
        Ok(Vec::new())
    }
    
    pub fn search_recipes(&self, query: &str) -> Vec<String> {
        let available = self.list_available_recipes();
        let mut matches = Vec::new();
        
        for recipe in available {
            if recipe.contains(query) {
                matches.push(recipe);
            }
        }
        
        matches
    }
}