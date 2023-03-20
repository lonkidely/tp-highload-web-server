#include <thread_pool.h>
#include <handler.h>
#include <pthread.h>

int socket_queue[THREAD_POOL_MAX_SOCKETS_COUNT];
int sockets_count = 0;

pthread_mutex_t mutex_queue;
pthread_cond_t cond_queue;

void add_socket(int socket) {
    pthread_mutex_lock(&mutex_queue);

    if (sockets_count == THREAD_POOL_MAX_SOCKETS_COUNT) {
        pthread_mutex_unlock(&mutex_queue);
        return;
    }

    socket_queue[sockets_count] = socket;
    sockets_count++;

    pthread_mutex_unlock(&mutex_queue);
    pthread_cond_broadcast(&cond_queue);
}

void *work_thread(void *args) {
    while (1) {
        pthread_mutex_lock(&mutex_queue);
        while (sockets_count == 0) {
            pthread_cond_wait(&cond_queue, &mutex_queue);
        }

        int socket = socket_queue[0];
        for (int i = 0; i < sockets_count - 1; i++) {
            socket_queue[i] = socket_queue[i + 1];
        }
        sockets_count--;

        pthread_mutex_unlock(&mutex_queue);

        handle(socket);
    }
}

void init_thread_pool() {
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_cond_init(&cond_queue, NULL);
}

void delete_thread_pool() {
    pthread_mutex_destroy(&mutex_queue);
    pthread_cond_destroy(&cond_queue);
}
