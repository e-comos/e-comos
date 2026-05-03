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

pub struct Package {
    pub name: String,
    pub version: String,
    pub url: String,
    pub size: u64,
}

pub enum DownloadError {
    NetworkError,
    InvalidUrl,
    FileError,
    ChecksumMismatch,
}
pub struct Downloader {
    pub cache_dir: String,
}

impl Downloader {
    pub fn new(cache_dir: String) -> Self {
        Self { cache_dir }
    }
    
    pub fn download_package(&self, package: &Package) -> Result<String, DownloadError> {
        // Build download path
        let filename = self.build_filename(package);
        let local_path = self.build_path(&filename);
        
        // Check if already cached
        if self.is_cached(&local_path) {
            return Ok(local_path);
        }
        
        // Execute download
        self.fetch_file(&package.url, &local_path)?;
        
        Ok(local_path)
    }
    
    fn build_filename(&self, package: &Package) -> String {
        let mut filename = String::new();
        filename.push_str(&package.name);
        filename.push('-');
        filename.push_str(&package.version);
        filename.push_str(".epm");
        filename
    }
    
    fn build_path(&self, filename: &str) -> String {
        let mut path = String::new();
        path.push_str(&self.cache_dir);
        path.push('/');
        path.push_str(filename);
        path
    }
    
    fn is_cached(&self, path: &str) -> bool {
        // Check if file exists by attempting to read it
        self.file_exists(path)
    }

    fn fetch_file(&self, url: &str, path: &str) -> Result<(), DownloadError> {
        // Parse URL and download file
        let (host, port, file_path) = self.parse_url(url)?;
        
        // Create HTTP request
        let request = self.build_http_request(&host, &file_path);
        
        // Send request and receive response
        let response_data = self.send_http_request(&host, port, &request)?;
        
        // Extract body from HTTP response
        let body = self.extract_http_body(&response_data)?;
        
        // Write to file
        self.write_file(path, &body)?;
        
        Ok(())
    }

    pub fn verify_checksum(&self, path: &str, expected: &str) -> bool {
        // Read file and calculate checksum
        if let Ok(data) = self.read_file(path) {
            let calculated = self.calculate_checksum(&data);
            calculated == expected
        } else {
            false
        }
    }
    
    // Helper functions for file and network operations
    fn file_exists(&self, _path: &str) -> bool {
        // Platform-specific file existence check
        // TODO: Implement actual file existence check via syscalls
        false
    }
    
    fn parse_url(&self, url: &str) -> Result<(String, u16, String), DownloadError> {
        // Parse HTTP URL into components
        if !url.starts_with("http://") && !url.starts_with("https://") {
            return Err(DownloadError::InvalidUrl);
        }
        
        let without_protocol = if url.starts_with("https://") {
            &url[8..]
        } else {
            &url[7..]
        };
        
        let parts: Vec<&str> = without_protocol.splitn(2, '/').collect();
        let host_port = parts[0];
        let path = if parts.len() > 1 {
            String::from("/") + parts[1]
        } else {
            String::from("/")
        };
        
        let (host, port) = if host_port.contains(':') {
            let hp: Vec<&str> = host_port.splitn(2, ':').collect();
            (String::from(hp[0]), hp[1].parse().unwrap_or(80))
        } else {
            (String::from(host_port), if url.starts_with("https") { 443 } else { 80 })
        };
        
        Ok((host, port, path))
    }
    
    fn build_http_request(&self, host: &str, path: &str) -> String {
        let mut request = String::new();
        request.push_str("GET ");
        request.push_str(path);
        request.push_str(" HTTP/1.1\r\n");
        request.push_str("Host: ");
        request.push_str(host);
        request.push_str("\r\n");
        request.push_str("Connection: close\r\n");
        request.push_str("\r\n");
        request
    }
    
    fn send_http_request(&self, _host: &str, _port: u16, _request: &str) -> Result<Vec<u8>, DownloadError> {
        // Platform-specific TCP socket implementation
        // TODO: Implement actual HTTP request via raw sockets
        Ok(Vec::new())
    }
    
    fn extract_http_body(&self, response: &[u8]) -> Result<Vec<u8>, DownloadError> {
        // Find end of HTTP headers (\r\n\r\n)
        for i in 0..response.len().saturating_sub(4) {
            if response[i..i+4] == [b'\r', b'\n', b'\r', b'\n'] {
                return Ok(response[i+4..].to_vec());
            }
        }
        Err(DownloadError::NetworkError)
    }
    
    fn write_file(&self, _path: &str, _data: &[u8]) -> Result<(), DownloadError> {
        // Platform-specific file writing via syscalls
        // TODO: Implement actual file writing
        Ok(())
    }
    
    fn read_file(&self, _path: &str) -> Result<Vec<u8>, DownloadError> {
        // Platform-specific file reading via syscalls
        // TODO: Implement actual file reading
        Ok(Vec::new())
    }
    
    fn calculate_checksum(&self, data: &[u8]) -> String {
        // Simple checksum calculation (sum of bytes)
        let sum: u32 = data.iter().map(|&b| b as u32).sum();
        let mut result = String::new();
        let hex_chars = b"0123456789abcdef";
        
        for i in (0..8).rev() {
            let nibble = (sum >> (i * 4)) & 0xf;
            result.push(hex_chars[nibble as usize] as char);
        }
        
        result
    }
}