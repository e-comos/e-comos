#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_lookup_info() {
    printf("\n=== LOOKUP(1) ===\n\n");
    printf("NAME\n");
    printf("    lookup - list directory contents\n\n");
    printf("SYNOPSIS\n");
    printf("    lookup [DIRECTORY]\n\n");
    printf("DESCRIPTION\n");
    printf("    List information about files and directories. Similar to ls -l.\n");
    printf("    If DIRECTORY is not specified, list current directory.\n\n");
    printf("    Output format: permissions size name\n\n");
    printf("EXAMPLES\n");
    printf("    lookup          List current directory\n");
    printf("    lookup /home    List /home directory\n\n");
    printf("SEE ALSO\n");
    printf("    saw(1), del(1), goto(1)\n\n");
}

void show_saw_info() {
    printf("\n=== SAW(1) ===\n\n");
    printf("NAME\n");
    printf("    saw - display file contents\n\n");
    printf("SYNOPSIS\n");
    printf("    saw FILE\n\n");
    printf("DESCRIPTION\n");
    printf("    Display the contents of FILE to standard output. Similar to cat.\n");
    printf("    The entire file is read and displayed.\n\n");
    printf("EXAMPLES\n");
    printf("    saw file.txt    Display contents of file.txt\n");
    printf("    saw README.md   Display README.md\n\n");
    printf("SEE ALSO\n");
    printf("    lookup(1), output(1)\n\n");
}

void show_del_info() {
    printf("\n=== DEL(1) ===\n\n");
    printf("NAME\n");
    printf("    del - delete files and directories\n\n");
    printf("SYNOPSIS\n");
    printf("    del FILE|DIRECTORY\n\n");
    printf("DESCRIPTION\n");
    printf("    Remove files and directories. Similar to rm.\n");
    printf("    WARNING: This permanently deletes files. Use with caution.\n\n");
    printf("EXAMPLES\n");
    printf("    del file.txt    Delete file.txt\n");
    printf("    del mydir       Delete empty directory mydir\n\n");
    printf("SEE ALSO\n");
    printf("    lookup(1), copy(1)\n\n");
}

void show_copy_info() {
    printf("\n=== COPY(1) ===\n\n");
    printf("NAME\n");
    printf("    copy - copy or move files\n\n");
    printf("SYNOPSIS\n");
    printf("    copy SOURCE DEST [move]\n\n");
    printf("DESCRIPTION\n");
    printf("    Copy SOURCE to DEST. Similar to cp.\n");
    printf("    If 'move' is specified as third argument, move instead of copy.\n\n");
    printf("EXAMPLES\n");
    printf("    copy file1.txt file2.txt      Copy file1.txt to file2.txt\n");
    printf("    copy old.txt new.txt move     Move old.txt to new.txt\n\n");
    printf("SEE ALSO\n");
    printf("    del(1), lookup(1)\n\n");
}

void show_goto_info() {
    printf("\n=== GOTO(1) ===\n\n");
    printf("NAME\n");
    printf("    goto - change directory\n\n");
    printf("SYNOPSIS\n");
    printf("    goto [DIRECTORY]\n\n");
    printf("DESCRIPTION\n");
    printf("    Change current working directory. Similar to cd.\n");
    printf("    If DIRECTORY is not specified, change to home directory.\n\n");
    printf("EXAMPLES\n");
    printf("    goto /home      Change to /home\n");
    printf("    goto            Change to home directory\n");
    printf("    goto ..         Change to parent directory\n\n");
    printf("SEE ALSO\n");
    printf("    lookup(1)\n\n");
}

void show_output_info() {
    printf("\n=== OUTPUT(1) ===\n\n");
    printf("NAME\n");
    printf("    output - display text\n\n");
    printf("SYNOPSIS\n");
    printf("    output [TEXT...]\n\n");
    printf("DESCRIPTION\n");
    printf("    Display TEXT to standard output. Similar to echo.\n");
    printf("    All arguments are printed separated by spaces.\n\n");
    printf("EXAMPLES\n");
    printf("    output Hello World     Print 'Hello World'\n");
    printf("    output \"Test message\"   Print 'Test message'\n\n");
    printf("SEE ALSO\n");
    printf("    saw(1)\n\n");
}

void show_info_of_info() {
    printf("\n=== INFO-OF(1) ===\n\n");
    printf("NAME\n");
    printf("    info-of - display command information\n\n");
    printf("SYNOPSIS\n");
    printf("    info-of [COMMAND]\n\n");
    printf("DESCRIPTION\n");
    printf("    Display detailed information about E-comOS commands. Similar to GNU info.\n");
    printf("    If COMMAND is not specified, show list of available commands.\n\n");
    printf("EXAMPLES\n");
    printf("    info-of         Show all commands\n");
    printf("    info-of lookup  Show lookup command info\n\n");
    printf("SEE ALSO\n");
    printf("    All E-comOS commands\n\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("E-comOS CommandBox Information System\n");
        printf("Usage: info-of COMMAND\n\n");
        printf("Available commands:\n");
        printf("  lookup    - List directory contents\n");
        printf("  saw       - Display file contents\n");
        printf("  del       - Delete files/directories\n");
        printf("  copy      - Copy or move files\n");
        printf("  goto      - Change directory\n");
        printf("  output    - Display text\n");
        printf("  info-of   - Show command information\n\n");
        printf("Use 'info-of COMMAND' for detailed information.\n");
        return 0;
    }
    
    if (strcmp(argv[1], "lookup") == 0) {
        show_lookup_info();
    } else if (strcmp(argv[1], "saw") == 0) {
        show_saw_info();
    } else if (strcmp(argv[1], "del") == 0) {
        show_del_info();
    } else if (strcmp(argv[1], "copy") == 0) {
        show_copy_info();
    } else if (strcmp(argv[1], "goto") == 0) {
        show_goto_info();
    } else if (strcmp(argv[1], "output") == 0) {
        show_output_info();
    } else if (strcmp(argv[1], "info-of") == 0) {
        show_info_of_info();
    } else {
        printf("No information available for '%s'\n", argv[1]);
        printf("Use 'info-of' without arguments to see available commands.\n");
        return 1;
    }
    
    return 0;
}