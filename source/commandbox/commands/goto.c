#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    const char *path = (argc > 1) ? argv[1] : getenv("HOME");
    
    if (chdir(path) != 0) {
        printf("Error: Cannot change to directory %s: %s\n", path, strerror(errno));
        return 1;
    }
    
    char current_dir[1024];
    getcwd(current_dir, sizeof(current_dir));
    printf("Current directory: %s\n", current_dir);
    return 0;
}