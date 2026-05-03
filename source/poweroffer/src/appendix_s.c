#include "appendix_s.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void handle_appendix_s_wait(int sender_pid, const char* data, int len) {
    printf("Handling WAIT message from PID %d\n", sender_pid);
    // Simulate granting time
    int granted_time_ms = 100;
    printf("Granted %d ms to PID %d\n", granted_time_ms, sender_pid);
}

void handle_appendix_s_okthanks(int sender_pid, const char* data, int len) {
    printf("Handling OKTHANKS message from PID %d\n", sender_pid);
    // Simulate marking process as completed
    printf("Process %d completed its shutdown sequence.\n", sender_pid);
}