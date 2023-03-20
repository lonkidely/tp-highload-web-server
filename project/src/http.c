#include <http.h>
#include <logger.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

const char *get_content_type(const char *filename) {
    size_t pos_dot = 0;
    for (size_t curr_pos = 0; filename[curr_pos] != '\0'; curr_pos++) {
        if (filename[curr_pos] == '.') {
            pos_dot = curr_pos;
        }
    }

    for (int i = 0; i < sizeof(ctype_dict) / sizeof(ctype_dict[0]) - 1; i++) {
        if (strcmp(filename + pos_dot + 1, ctype_dict[i].extension) == 0) {
            return ctype_dict[i].ctype;
        }
    }

    return "";
}

const char* get_status_code_msg(int status_code) {
    switch (status_code) {
        case HTTP_STATUS_OK:
            return "OK";
        case HTTP_STATUS_FORBIDDEN:
            return "Forbidden";
        case HTTP_STATUS_NOT_FOUND:
            return "Not Found";
        case HTTP_STATUS_METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        default:
            return "Unknown HTTP Status Code";
    }
}

void set_headers_http_ok(int status_code, int content_length, const char *filename, char *headers) {
    char template[] =
            "%s %d %s\r\nServer: server\r\nContent-Length: %d\r\nContent-Type: %s\r\nConnection: Closed\r\n\r\n";

    sprintf(headers, template,
            "HTTP/1.1", HTTP_STATUS_OK, get_status_code_msg(status_code), content_length, get_content_type(filename));
}

void set_headers_http_error(int status_code, char *headers) {
    char template[] =
            "%s %d %s\r\nServer: server\r\nContent-Length: %d\r\nContent-Type: %s\r\nConnection: Closed\r\n\r\n";

    sprintf(headers, template,
            "HTTP/1.1", status_code, get_status_code_msg(status_code), 0, "text/plain");
}

void send_err_response(int socket, int status_code) {
    char error_msg[HTTP_MAX_MESSAGE_LENGTH];

    set_headers_http_error(status_code, error_msg);

    int bytes_sent = send(socket, error_msg, strlen(error_msg), 0);
    if (bytes_sent == -1) {
        LOG_ERROR("socket send failed (send_err_response)");
    }
}
