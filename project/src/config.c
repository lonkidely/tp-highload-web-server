#include "config.h"

int read_config(char *path, server_config *config) {
    if (!path) {
        printf("[ERROR] path is null (read_config)");
        return EXIT_FAILURE;
    }

    FILE* file = fopen(path, "r");
    if (!file) {
        printf("[ERROR] file is null (read_config)");
        return EXIT_FAILURE;
    }

    while (!feof(file)) {
        char buf[MAX_LINE_LENGTH];
        if (!fgets(buf, MAX_LINE_LENGTH - 1, file) && !feof(file)) {
            printf("[ERROR] fgets error (read_config)");
            fclose(file);
            return EXIT_FAILURE;
        }

        printf("Current line of config = %s", buf);
    }

    fclose(file);

    return EXIT_SUCCESS;
}
