#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int new_config(server_config **cfg) {
    if (!cfg) {
        printf("[ERROR] null pointer instead of cfg (new_config)\n");
        return EXIT_FAILURE;
    }

    if (*cfg != NULL) {
        printf("[ERROR] config already created (new_config)\n");
        return EXIT_FAILURE;
    }

    *cfg = (server_config *) malloc(sizeof(server_config));

    if (*cfg == NULL) {
        printf("[ERROR] malloc for config failed (new_config)\n");
        return EXIT_FAILURE;
    }

    // Default values
    (*cfg)->port = 0;
    (*cfg)->thread_limit = 0;
    (*cfg)->document_root = NULL;

    return EXIT_SUCCESS;
}

void write_config(server_config *config) {
    if (!config) {
        return;
    }

    printf("Server config:\n");
    printf("Port = %d\n", config->port);
    printf("Thread limit = %d\n", config->thread_limit);
    if (config->document_root) {
        printf("Document root = %s\n", config->document_root);
    }
}

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

        if (feof(file)) {
            break;
        }

        char *state;
        char *key = strtok_r(buf, " ", &state);
        char *value = strtok_r(NULL, " ", &state);
        if (!key || !value) {
            printf("[ERROR] cant split line (read_config)\n");
            fclose(file);
            return EXIT_FAILURE;
        }

        if (strcmp(key, CONFIG_THREAD_LIMIT_KEY) == 0) {
            config->thread_limit = atoi(value);
        } else if (strcmp(key, CONFIG_DOCUMENT_ROOT_KEY) == 0) {
            config->document_root = strdup(value);
        } else if (strcmp(key, CONFIG_PORT_KEY) == 0) {
            config->port = atoi(value);
        }
    }

    fclose(file);

    return EXIT_SUCCESS;
}

void delete_config(server_config *cfg) {
    if (!cfg) {
        return;
    }

    if (cfg->document_root) {
        free(cfg->document_root);
    }

    free(cfg);
}
