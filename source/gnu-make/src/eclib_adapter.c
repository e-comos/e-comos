/* ECLib adapter implementation for E-comOS */

#include "eclib_adapter.h"
#include <eclib/men.h>
#include <eclib/utils.h>

/* Convert Unix path to E-comOS path (/ -> >) */
char *ecomos_path(const char *unix_path) {
    if (!unix_path) return NULL;
    
    size_t len = eclib_strlen(unix_path);
    char *result = eclib_malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i <= len; i++) {
        result[i] = (unix_path[i] == UNIX_PATH_SEP) ? ECOMOS_PATH_SEP : unix_path[i];
    }
    
    return result;
}

/* Convert E-comOS path to Unix path (> -> /) */
char *unix_path(const char *ecomos_path) {
    if (!ecomos_path) return NULL;
    
    size_t len = eclib_strlen(ecomos_path);
    char *result = eclib_malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i <= len; i++) {
        result[i] = (ecomos_path[i] == ECOMOS_PATH_SEP) ? UNIX_PATH_SEP : ecomos_path[i];
    }
    
    return result;
}
