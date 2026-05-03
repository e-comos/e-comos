#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: copy <source> <destination> [move]\n");
        return 1;
    }
    
    FILE *src = fopen(argv[1], "rb");
    if (!src) {
        printf("Error: Cannot open source file %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    
    FILE *dst = fopen(argv[2], "wb");
    if (!dst) {
        printf("Error: Cannot create destination file %s: %s\n", argv[2], strerror(errno));
        fclose(src);
        return 1;
    }
    
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }
    
    fclose(src);
    fclose(dst);
    
    // Check if it's a move operation
    if (argc > 3 && strcmp(argv[3], "move") == 0) {
        unlink(argv[1]);
        printf("Moved: %s -> %s\n", argv[1], argv[2]);
    } else {
        printf("Copied: %s -> %s\n", argv[1], argv[2]);
    }
    
    return 0;
}