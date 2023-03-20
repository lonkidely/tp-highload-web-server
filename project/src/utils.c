#include <logger.h>
#include <utils.h>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void remove_query_params(char *src) {
    char *query_param_pos = strstr(src, "?");
    if (query_param_pos != NULL) {
        *query_param_pos = '\0';
    }
}

int decode_url(const char *src, char *dst) {
    if (dst == NULL) {
        LOG_ERROR("null instead of dst (decode_url)");
        return EXIT_FAILURE;
    }

    int i = 0, j = 0;
    while (src[i] != '\0') {
        if (src[i] == '%' && src[i + 1] != '\0' && src[i + 2] != '\0') {
            char hex[3];
            hex[0] = src[i + 1];
            hex[1] = src[i + 2];
            hex[2] = '\0';

            int ascii = strtol(hex, NULL, 16);
            dst[j] = (char)ascii;

            i += 3;
        } else {
            dst[j] = src[i];
            i++;
        }
        j++;
    }

    dst[j] = '\0';

    return EXIT_SUCCESS;
}

int is_directory(const char *path) {
    return path[strlen(path) - 1] == '/';
}

ssize_t get_content_length(const char *filename) {
    struct stat statistics;
    if (stat(filename, &statistics) == -1) {
        return -1;
    }
    return statistics.st_size;
}
