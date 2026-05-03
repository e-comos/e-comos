#include <eclib/utils.h>
#include <eclib/file.h>

int main(int argc, char **argv) {
    const char *msg = "Hello from E-comOS Make!\n";
    eclib_file_t stdout_file;
    
    /* Open stdout */
    stdout_file.fd = 1;
    
    /* Write message */
    eclib_file_write(&stdout_file, msg, eclib_strlen(msg));
    
    return 0;
}
