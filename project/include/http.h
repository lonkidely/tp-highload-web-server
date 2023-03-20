#ifndef TP_HIGHLOAD_WEB_SERVER_HTTP_H
#define TP_HIGHLOAD_WEB_SERVER_HTTP_H

#define HTTP_MAX_REQUEST_LENGTH 2048
#define HTTP_MAX_MESSAGE_LENGTH 2048
#define HTTP_MAX_QUERY_TYPE_LENGTH 32
#define HTTP_MAX_URL_LENGTH 2048
#define HTTP_MAX_HTTP_VERSION_LENGTH 32

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_FORBIDDEN 403
#define HTTP_STATUS_NOT_FOUND 404
#define HTTP_STATUS_METHOD_NOT_ALLOWED 405

static const struct dictionary_extension_ctype {
    const char *extension;
    const char *ctype;
} ctype_dict[] = {
        {"html", "text/html"},
        {"htm", "text/htm"},
        {"php", "text/html"},
        {"css", "text/css"},
        {"js", "application/javascript"},
        {"gif", "image/gif"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"png", "image/png"},
        {"swf", "application/x-shockwave-flash"},
        {"pdf", "application/pdf"},
        {"ps", "application/postscript"},
        {"txt", "text/plain"},
        {"ico", "image/x-icon"}
};

const char *get_content_type(const char *filename);
const char* get_status_code_msg(int code);

void set_headers_http_ok(int status_code, int content_length, const char *filename, char *headers);
void set_headers_http_error(int status_code, char *headers);
void send_err_response(int socket, int status_code);

#endif
