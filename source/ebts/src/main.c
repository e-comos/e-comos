/**
 * ebts - E-comOS Basic Terminal Shell
 * Copyright (C) 2025,2026 E-comOS Project
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
#include "config.h"
#include "shell/shell.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

int main(void);

#ifdef ECOMOS_BUILD
#include "ecomos_types.h"
#include "syscalls.h"
#include "eclib/ipc_message.h"

// E-comOS entry point
void ebts_service_entry(void) {
    // Register as shell service
    struct ipc_message msg;
    msg.sender = SERVICE_SHELL;
    msg.size = 40;
    msg.data[0] = MSG_SERVICE_REGISTER;
    msg.data[1] = SERVICE_SHELL;
    strcpy((char*)&msg.data[8], "ebts_shell");
    ipc_send(0, &msg);
    
    // Start shell
    main();
}
#endif

int main(void) {
    time_t now;
    struct tm *timeinfo;
    char timestr[64];
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M", timeinfo);
    
    printf("Welcome to ebts version 1.\n");
    printf("Last log in is on %s\n", timestr);
    
    shell_init();
    shell_run();
    shell_cleanup();
    
    return 0;
}
