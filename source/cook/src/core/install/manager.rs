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
use super::installer::{Installer, InstallContext, InstallError};
use super::downloader::Package;

pub struct PackageMetadata {
    pub name: String,
    pub version: String,
    pub url: String,
    pub size: u64,
}

pub struct InstallManager {
    pub installer: Installer,
    pub repositories: Vec<String>,
    pub repo_url: String,
}

impl InstallManager {
    pub fn new(cache_dir: String, install_root: String) -> Self {
        Self {
            installer: Installer::new(cache_dir, install_root),
            repositories: Vec::new(),
            repo_url: String::new(),
        }
    }
    
    pub fn add_repository(&mut self, repo_url: String) {
        self.repositories.push(repo_url);
    }
    
    pub fn install(&self, package_name: &str, version: Option<&str>) -> Result<(), InstallError> {
        // 1. Find package
        let package = self.find_package(package_name, version)?;
        
        // 2. Create install context
        let context = InstallContext {
            package,
            install_dir: self.build_install_path(package_name),
            force_install: false,
        };
        
        // 3. Execute installation
        self.installer.install_package(context)
    }
    
    pub fn force_install(&self, package_name: &str, version: Option<&str>) -> Result<(), InstallError> {
        let package = self.find_package(package_name, version)?;
        
        let context = InstallContext {
            package,
            install_dir: self.build_install_path(package_name),
            force_install: true,
        };
        
        self.installer.install_package(context)
    }
    
    pub fn uninstall(&self, package_name: &str) -> Result<(), InstallError> {
        self.installer.uninstall_package(package_name)
    }
    
    fn find_package(&self, package_name: &str, version: Option<&str>) -> Result<Package, InstallError> {
        // Simplified: construct package info
        let version_str = version.unwrap_or("latest");
        
        Ok(Package {
            name: String::from(package_name),
            version: String::from(version_str),
            url: self.build_package_url(package_name, version_str),
            size: 0,
        })
    }
    
    fn build_package_url(&self, package_name: &str, version: &str) -> String {
        let mut url = String::new();
        if !self.repositories.is_empty() {
            url.push_str(&self.repositories[0]);
        } else {
            url.push_str("https://github.com/E-comOS-Operation-System/recipe");
        }
        url.push('/');
        url.push_str(package_name);
        url.push('/');
        url.push_str(version);
        url.push_str(".cook");
        url
    }
    
    fn build_install_path(&self, package_name: &str) -> String {
        let mut path = String::new();
        path.push_str(&self.installer.install_root);
        path.push('/');
        path.push_str(package_name);
        path
    }
}