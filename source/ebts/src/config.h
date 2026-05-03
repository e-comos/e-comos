/**
 * ebts - E-comOS Basic Terminal Shell
 * Copyright (C) 2025 E-comOS Project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef CONFIG_H
#define CONFIG_H

// System version information
#define ECOS_VERSION "1.0.0"
#define ECOS_BUILD_DATE __DATE__

// System limits
#define MAX_INPUT_LENGTH 1024
#define MAX_PATH_LENGTH 512
#define MAX_OBJECT_NAME 256
#define MAX_COMMAND_ARGS 10
#define MAX_OBJECTS 1000
#define MAX_CHILDREN 50

// Object types for E-comOS philosophy
typedef enum {
    OBJ_TYPE_FILE,      // File object (contains data)
    OBJ_TYPE_FOLDER     // Folder object (contains other objects)
} object_type_t;

// Error codes for robust error handling
typedef enum {
    EC_SUCCESS = 0,             // Operation completed successfully
    EC_ERROR = -1,              // General error occurred
    EC_INVALID_COMMAND = -2,    // Command not recognized
    EC_OBJECT_NOT_FOUND = -3,   // Requested object does not exist
    EC_NOT_A_FOLDER = -4,       // Object is not a folder type
    EC_NOT_A_FILE = -5,         // Object is not a file type
    EC_INVALID_PATH = -6,       // Path format is invalid
    EC_MEMORY_ERROR = -7        // Memory allocation failed
} error_code_t;

#endif /* CONFIG_H */