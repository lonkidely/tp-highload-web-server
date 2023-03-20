#ifndef TP_HIGHLOAD_WEB_SERVER_UTILS_H
#define TP_HIGHLOAD_WEB_SERVER_UTILS_H

#include <sys/types.h>

void remove_query_params(char *src);
int decode_url(const char *src, char *dst);

int is_directory(const char *path);
ssize_t get_content_length(const char *filename);

#endif
