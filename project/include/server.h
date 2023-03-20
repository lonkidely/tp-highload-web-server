#ifndef TP_HIGHLOAD_WEB_SERVER_SERVER_H
#define TP_HIGHLOAD_WEB_SERVER_SERVER_H

#include <config.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_DEFAULT_PORT 80
#define SERVER_DEFAULT_ROOT_DIR "/var/www/html"
#define SERVER_DEFAULT_ROOT_DIR_SIZE 14

typedef struct server {
    int server_socket;
    struct sockaddr_in server_addr;
    char *root_dir;
} server;

int new_server(server **serv);
int configure_server(server *serv, server_config *cfg);
int run_server(server *serv);
void delete_server(server *serv);


#endif
