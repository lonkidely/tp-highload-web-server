#include <handler.h>
#include <http.h>
#include <logger.h>
#include <utils.h>

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <unistd.h>

void handle(int client_socket, char *root_dir) {
    char request[HTTP_MAX_REQUEST_LENGTH];

    ssize_t accepted = recv(client_socket, request, sizeof(request), 0);
    if (accepted < 0) {
        LOG_ERROR("recv failed (handle)");
        close(client_socket);
        return;
    }

    request[accepted] = '\0';

    char query_type[HTTP_MAX_QUERY_TYPE_LENGTH];
    char url[HTTP_MAX_URL_LENGTH];
    char http_version[HTTP_MAX_HTTP_VERSION_LENGTH];

    if (sscanf(request, "%s %s %s", query_type, url, http_version) != 3) {
        LOG_ERROR("parse url failed (handle)");
        close(client_socket);
        return;
    }

    if (strcmp(query_type, "GET") && strcmp(query_type, "HEAD")) {
        send_err_response(client_socket, HTTP_STATUS_METHOD_NOT_ALLOWED);
        close(client_socket);
        return;
    }

    char tmp_path[HTTP_MAX_URL_LENGTH];

    if (decode_url(url, tmp_path) != EXIT_SUCCESS) {
        close(client_socket);
        return;
    }

    char path[HTTP_MAX_URL_LENGTH];

    strcpy(path, root_dir);
    strcat(path, tmp_path);

    remove_query_params(path);

    int is_dir = is_directory(path);

    char resolved_path[HTTP_MAX_URL_LENGTH];
    if (realpath(path, resolved_path) == NULL) {
        if (is_dir && errno != ENOTDIR) {
            LOG_ERROR("is and not directory (realpath, handle)");
            LOG_ERROR(resolved_path);

            send_err_response(client_socket, HTTP_STATUS_FORBIDDEN);
            close(client_socket);
            return;
        }

        LOG_ERROR("is directory and not found (realpath, handle)");
        LOG_ERROR(path);
        LOG_ERROR(resolved_path);

        send_err_response(client_socket, HTTP_STATUS_NOT_FOUND);
        close(client_socket);
        return;
    }

    if (strncmp(root_dir, resolved_path, strlen(root_dir)) != 0) {
        LOG_ERROR("ctrncmp failed (handle)");
        LOG_ERROR(root_dir);
        LOG_ERROR(resolved_path);

        send_err_response(client_socket, HTTP_STATUS_NOT_FOUND);
        close(client_socket);
        return;
    }

    if (is_dir) {
        strcat(path, INDEX_FILE);
    }

    FILE *file = fopen(path, "r");

    if (!file) {
        if (is_dir) {
            LOG_ERROR("file is null and is dir (handle)");
            send_err_response(client_socket, HTTP_STATUS_FORBIDDEN);
        } else {
            LOG_ERROR("file is null and is file (handle)");
            send_err_response(client_socket, HTTP_STATUS_NOT_FOUND);
        }

        close(client_socket);
        return;
    }

    char response[HTTP_MAX_MESSAGE_LENGTH];

    int content_length = get_content_length(path);

    set_headers_http_ok(HTTP_STATUS_OK, content_length, path, response);

    send(client_socket, response, strlen(response), 0);

    if (strcmp(query_type, "HEAD") == 0) {
        fclose(file);
        close(client_socket);
        return;
    }

    int file_fd = fileno(file);
    int bytes_sent = 0;
    int curr_sent = 0;
    while (bytes_sent < content_length) {
        curr_sent = sendfile(client_socket, file_fd, NULL, content_length);

        if (curr_sent == -1) {
            break;
        }
        bytes_sent += curr_sent;
    }

    fclose(file);

    close(client_socket);
}
