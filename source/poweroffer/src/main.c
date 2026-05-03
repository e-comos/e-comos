#include "poweroffer.h"
#include <stdio.h>

int main() {
    printf("PowerOffer Shutdown Service Starting...\n");
    if (poweroffer_init() != 0) {
        fprintf(stderr, "Failed to initialize PowerOffer service.\n");
        return 1;
    }
    printf("PowerOffer Shutdown Service Initialized.\n");
    return 0;
}