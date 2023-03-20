#include <config.h>
#include <logger.h>
#include <server.h>
#include <utils.h>
#include <handler.h>
#include <thread_pool.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


int main(int argc, char *argv[]) {
    server_config *cfg = NULL;

    if (new_config(&cfg) != EXIT_SUCCESS) {
        delete_config(cfg);
        return EXIT_FAILURE;
    }

    if (read_config(argv[1], cfg) != EXIT_SUCCESS) {
        delete_config(cfg);
        return EXIT_FAILURE;
    }

    write_config(cfg);

    init_handler(cfg->document_root);

    server *serv = NULL;

    if (new_server(&serv) != EXIT_SUCCESS) {
        delete_server(serv);
        return EXIT_FAILURE;
    }

    if (configure_server(serv, cfg) != EXIT_SUCCESS) {
        delete_server(serv);
        return EXIT_FAILURE;
    }

    LOG_INFO("Server created! (main)");

    init_thread_pool();

    pthread_t th[cfg->thread_limit];
    for (int i = 0; i < cfg->thread_limit; i++) {
        if (pthread_create(&th[i], NULL, &work_thread, NULL) != 0) {
            LOG_ERROR("Failed to create the thread");
        }
    }

    run_server(serv);

    delete_config(cfg);
    delete_server(serv);

    delete_thread_pool();

    LOG_INFO("Server stopped successfully");

    return 0;
}
