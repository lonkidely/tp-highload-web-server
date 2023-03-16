#ifndef TP_HIGHLOAD_WEB_SERVER_CONFIG_H
#define TP_HIGHLOAD_WEB_SERVER_CONFIG_H

#include "stdio.h"
#include "stdlib.h"

#define MAX_LINE_LENGTH 128

typedef struct server_config {
    int thread_limit;
    char *document_root;
} server_config;

int read_config(char *path, server_config *config);

#endif
