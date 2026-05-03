#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char current_hostname[256];
    
    // Get current hostname
    if (gethostname(current_hostname, sizeof(current_hostname)) != 0) {
        printf("Error: Cannot get current hostname: %s\n", strerror(errno));
        return 1;
    }
    
    // If no arguments, just display current hostname
    if (argc == 1) {
        printf("%s\n", current_hostname);
        return 0;
    }
    
    // If argument provided, check for help or set new hostname
    if (argc == 2) {
        // Check for help flags
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("Usage: hostname [new_hostname]\n");
            printf("  hostname          - Display current hostname\n");
            printf("  hostname <name>   - Set new hostname\n");
            return 0;
        }
        
        if (sethostname(argv[1], strlen(argv[1])) != 0) {
            printf("Error: Cannot set hostname to %s: %s\n", argv[1], strerror(errno));
            printf("Note: Setting hostname may require root privileges\n");
            return 1;
        }
        printf("Hostname changed from %s to %s\n", current_hostname, argv[1]);
        return 0;
    }
    
    // Too many arguments
    printf("Usage: hostname [new_hostname]\n");
    printf("  hostname          - Display current hostname\n");
    printf("  hostname <name>   - Set new hostname\n");
    return 1;
}