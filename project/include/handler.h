#ifndef TP_HIGHLOAD_WEB_SERVER_HANDLER_H
#define TP_HIGHLOAD_WEB_SERVER_HANDLER_H

#define INDEX_FILE "index.html"

void init_handler(char *document_root);

void handle(int client_socket);

#endif
