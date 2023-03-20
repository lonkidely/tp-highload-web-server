#include <server.h>
#include <logger.h>
#include <http.h>
#include <handler.h>
#include <thread_pool.h>

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int new_server(server **serv) {
    if (!serv) {
        LOG_ERROR("null instead of server (new_server)");
        return EXIT_FAILURE;
    }

    if (*serv) {
        LOG_ERROR("server already created (new_server)");
        return EXIT_FAILURE;
    }

    *serv = (server *)malloc(sizeof(server));

    if (*serv == NULL) {
        LOG_ERROR("malloc for server failed (new_server)");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int configure_server(server *serv, server_config *cfg) {
    if (!serv) {
        LOG_ERROR("null instead of server (configure_server)");
        return EXIT_FAILURE;
    }

    serv->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (serv->server_socket < 0) {
        LOG_ERROR("create socket failed (configure_server)");
        return EXIT_FAILURE;
    }

    serv->server_addr.sin_family = AF_INET;
    serv->server_addr.sin_addr.s_addr = INADDR_ANY;

    if (cfg) {
        serv->server_addr.sin_port = htons(cfg->port);
    } else {
        serv->server_addr.sin_port = htons(SERVER_DEFAULT_PORT);
    }

    bind(serv->server_socket, (struct sockaddr *)&serv->server_addr, sizeof(serv->server_addr));

    if (fcntl(serv->server_socket, F_SETFL, fcntl(serv->server_socket, F_GETFL, 0) | O_NONBLOCK) != 0) {
        close(serv->server_socket);
        LOG_ERROR("fcntl failed (configure server)");
        return EXIT_FAILURE;
    }

    if (listen(serv->server_socket, INT_MAX) != 0) {
        close(serv->server_socket);
        LOG_ERROR("server listen failed (configure server)");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int run_server(server *serv) {
    while (1) {
        int client_socket = accept(serv->server_socket, NULL, NULL);

        if (client_socket < 0) {
            continue;
        }

        add_socket(client_socket);
    }

    return EXIT_SUCCESS;
}

void delete_server(server *serv) {
    if (!serv) {
        return;
    }
    close(serv->server_socket);
    free(serv);
}
