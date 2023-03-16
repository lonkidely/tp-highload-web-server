#include "stdio.h"
#include "config.h"

int main(int argc, char *argv[]) {
    printf("Argc count = %d\n", argc);

    server_config* cfg = malloc(sizeof(server_config));

    read_config(argv[1], cfg);

    free(cfg);

    return 0;
}