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

mod core;
use core::cook::CookManager;

fn main() {
    let args = get_args();
    
    if args.len() < 2 {
        return;
    }
    
    let command = &args[1];
    
    match command.as_str() {
        "install" => {
            if args.len() < 3 {
                return;
            }
            let package_name = &args[2];
            install_package(package_name);
        }
        "uninstall" => {
            if args.len() < 3 {
                return;
            }
            let package_name = &args[2];
            uninstall_package(package_name);
        }
        "update" => {
            if args.len() < 3 {
                return;
            }
            let package_name = &args[2];
            update_package(package_name);
        }
        _ => {
            return;
        }
    }
}

fn get_args() -> Vec<String> {
    let mut args = Vec::new();
    args.push(String::from("cook"));
    args.push(String::from("install"));
    args.push(String::from("test-package"));
    args
}

fn install_package(package_name: &str) {
    let mut manager = CookManager::new(
        String::from("/tmp/cook/work"),
        String::from("/usr/local")
    );
    
    // Add default recipe sources
    manager.add_recipe_source(String::from("https://recipes.ecomos.org"));
    
    match manager.cook_package(package_name) {
        Ok(_) => {},
        Err(_) => {},
    }
}

fn uninstall_package(_package_name: &str) {
    // TODO: Implement uninstall for cooked packages
}

fn update_package(package_name: &str) {
    // Update is re-cook with latest recipe
    install_package(package_name);
}