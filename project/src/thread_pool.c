#include <thread_pool.h>
#include <logger.h>
#include <handler.h>
#include <pthread.h>

int socket_queue[THREAD_POOL_MAX_SOCKETS_COUNT];
int sockets_count = 0;

pthread_mutex_t mutex_queue;
pthread_cond_t cond_queue;

pthread_t th[THREAD_MAX_THREADS_LIMIT];

void add_socket(int socket) {
    pthread_mutex_lock(&mutex_queue);

    if (sockets_count == THREAD_POOL_MAX_SOCKETS_COUNT) {
        pthread_mutex_unlock(&mutex_queue);
        return;
    }

    socket_queue[sockets_count] = socket;
    sockets_count++;

    pthread_mutex_unlock(&mutex_queue);
    pthread_cond_signal(&cond_queue);
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

void init_thread_pool(int thread_limit) {
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_cond_init(&cond_queue, NULL);

    for (int i = 0; i < thread_limit && i < THREAD_MAX_THREADS_LIMIT; i++) {
        if (pthread_create(&th[i], NULL, &work_thread, NULL) != 0) {
            LOG_ERROR("Failed to create the thread");
        }
    }
}

void delete_thread_pool() {
    pthread_mutex_destroy(&mutex_queue);
    pthread_cond_destroy(&cond_queue);
}
