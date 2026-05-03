#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
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

int main(int argc, char *argv[]) {
    const char *path = (argc > 1) ? argv[1] : ".";
    
    DIR *dir = opendir(path);
    if (!dir) {
        printf("Error: Cannot open directory %s: %s\n", path, strerror(errno));
        return 1;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        print_file_info(full_path, entry->d_name);
    }
    
    closedir(dir);
    return 0;
}