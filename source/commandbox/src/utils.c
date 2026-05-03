#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>


void print_file_info(const char *path, const char *name) {
    struct stat st;
    if (stat(path, &st) != 0) {
        printf("%-20s [error]\n", name);
        return;
    }
    
    char type = S_ISDIR(st.st_mode) ? 'd' : '-';
    char perms[10];
    snprintf(perms, sizeof(perms), "%c%c%c%c%c%c%c%c%c",
        type,
        (st.st_mode & S_IRUSR) ? 'r' : '-',
        (st.st_mode & S_IWUSR) ? 'w' : '-',
        (st.st_mode & S_IXUSR) ? 'x' : '-',
        (st.st_mode & S_IRGRP) ? 'r' : '-',
        (st.st_mode & S_IWGRP) ? 'w' : '-',
        (st.st_mode & S_IXGRP) ? 'x' : '-',
        (st.st_mode & S_IROTH) ? 'r' : '-',
        (st.st_mode & S_IWOTH) ? 'w' : '-',
        (st.st_mode & S_IXOTH) ? 'x' : '-'
    );
    
    printf("%s %8ld %s\n", perms, st.st_size, name);
}

int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

int is_directory(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}