#include "stdio.h"
#include "stdlib.h"
#include "config.h"

int main(int argc, char *argv[]) {
    server_config *cfg = NULL;

    if (new_config(&cfg)) {
        return EXIT_FAILURE;
    }

    if (read_config(argv[1], cfg)) {
        return EXIT_FAILURE;
    }

    write_config(cfg);

    delete_config(cfg);

    return 0;
}
