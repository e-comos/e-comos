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

pub struct Certificate {
    pub name: String,
    pub version_and_build_version: String,
    pub certificate_issuing_authority: String,
    pub certificate_issuance_date: String,
    pub development: String,
    pub valid_until: String,
    pub robot: Vec<String>,
    pub verified: String,
    pub validation_info: ValidationInfo,
}

pub struct ValidationInfo {
    pub verified: String,
    pub verification_date: String,
    pub verifier: String,
    pub signature: String,
    pub release_code: String,
    pub bot: String,
}

pub enum CertificateError {
    ParseError,
    InvalidFormat,
    Unverified,
    Expired,
}

impl Certificate {
    pub fn parse(content: &str) -> Result<Self, CertificateError> {
        let mut name = String::new();
        let mut version_and_build_version = String::new();
        let mut certificate_issuing_authority = String::new();
        let mut certificate_issuance_date = String::new();
        let mut development = String::new();
        let mut valid_until = String::new();
        let mut verified = String::new();
        let mut robot = Vec::new();
        
        let mut val_verified = String::new();
        let mut verification_date = String::new();
        let mut verifier = String::new();
        let mut signature = String::new();
        let mut release_code = String::new();
        let mut bot = String::new();
        
        let mut in_robot_section = false;
        let mut in_validation_section = false;
        
        for line in content.lines() {
            let line = line.trim();
            
            if line.starts_with("//") || line.is_empty() || line == "{" || line == "}" {
                continue;
            }
            
            if line.contains("[ROBOT]{") {
                in_robot_section = true;
                continue;
            }
            
            if line.contains("\"validation_info\"{") {
                in_validation_section = true;
                continue;
            }
            
            if in_robot_section {
                if line == "}" {
                    in_robot_section = false;
                    continue;
                }
                robot.push(String::from(line));
                continue;
            }
            
            if in_validation_section {
                if line == "}" {
                    in_validation_section = false;
                    continue;
                }
                if let Some(value) = Self::extract_field(line, "verified") {
                    val_verified = value;
                } else if let Some(value) = Self::extract_field(line, "verification_date") {
                    verification_date = value;
                } else if let Some(value) = Self::extract_field(line, "verifier") {
                    verifier = value;
                } else if let Some(value) = Self::extract_field(line, "signature") {
                    signature = value;
                } else if let Some(value) = Self::extract_field(line, "release_code") {
                    release_code = value;
                } else if let Some(value) = Self::extract_field(line, "bot") {
                    bot = value;
                }
                continue;
            }
            
            if let Some(value) = Self::extract_field(line, "name") {
                name = value;
            } else if let Some(value) = Self::extract_field(line, "version_and_build_version") {
                version_and_build_version = value;
            } else if let Some(value) = Self::extract_field(line, "certificate issuing authority") {
                certificate_issuing_authority = value;
            } else if let Some(value) = Self::extract_field(line, "certificate issuance date") {
                certificate_issuance_date = value;
            } else if let Some(value) = Self::extract_field(line, "development") {
                development = value;
            } else if let Some(value) = Self::extract_field(line, "valid until") {
                valid_until = value;
            } else if let Some(value) = Self::extract_field(line, "verified") {
                verified = value;
            }
        }
        
        let validation_info = ValidationInfo {
            verified: val_verified,
            verification_date,
            verifier,
            signature,
            release_code,
            bot,
        };
        
        Ok(Certificate {
            name,
            version_and_build_version,
            certificate_issuing_authority,
            certificate_issuance_date,
            development,
            valid_until,
            robot,
            verified,
            validation_info,
        })
    }
    
    fn extract_field(line: &str, field_name: &str) -> Option<String> {
        if let Some(start) = line.find('[') {
            if let Some(end) = line.find(']') {
                let key = &line[start + 1..end];
                if key == field_name {
                    if let Some(colon_start) = line[end..].find('[') {
                        if let Some(colon_end) = line[end + colon_start..].find(']') {
                            let value = &line[end + colon_start + 1..end + colon_start + colon_end];
                            return Some(String::from(value));
                        }
                    }
                }
            }
        }
        None
    }
    
    pub fn is_verified(&self) -> bool {
        self.verified == "verified" && self.validation_info.verified == "verified"
    }
    
    pub fn validate(&self) -> Result<(), CertificateError> {
        if !self.is_verified() {
            return Err(CertificateError::Unverified);
        }
        Ok(())
    }
}

pub fn prompt_unverified_package(_package_name: &str) -> u8 {
    2
}