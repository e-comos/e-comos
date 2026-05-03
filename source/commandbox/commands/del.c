#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int is_directory(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: del <file/directory>\n");
        return 1;
    }
    
    if (is_directory(argv[1])) {
        if (rmdir(argv[1]) != 0) {
            printf("Error: Cannot remove directory %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
    } else {
        if (unlink(argv[1]) != 0) {
            printf("Error: Cannot remove file %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
    }
    
    printf("Deleted: %s\n", argv[1]);
    return 0;
}