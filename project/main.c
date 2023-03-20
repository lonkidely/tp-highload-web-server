#include <config.h>
#include <logger.h>
#include <server.h>
#include <utils.h>

#include <stdio.h>
#include <stdlib.h>


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

    run_server(serv);

    delete_config(cfg);
    delete_server(serv);

    LOG_INFO("Server stopped successfully");

    return 0;
}
