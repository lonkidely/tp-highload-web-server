#ifndef TP_HIGHLOAD_WEB_SERVER_THREAD_POOL_H
#define TP_HIGHLOAD_WEB_SERVER_THREAD_POOL_H

#include <pthread.h>

#define THREAD_POOL_MAX_SOCKETS_COUNT 512
#define THREAD_MAX_THREADS_LIMIT 16

void add_socket(int socket);

void* work_thread(void *args);

void init_thread_pool(int thread_limit);
void delete_thread_pool(void);

#endif
