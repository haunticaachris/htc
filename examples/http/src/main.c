#define HTC_CRYPTO_IMPLEMENTATION 1
#define HTC_SOCKET_IMPLEMENTATION 1

#include "htc_alloc.h"
#include "htc_crypto.h"
#include "htc_file.h"
#include "htc_logger.h"
#include "htc_plugin.h"
#include "htc_socket.h"

#include <stdarg.h> /* Var args */
#include <stdio.h>  /* For io functions */
#include <string.h> /* For memcpy */

/* File Logger */
bool htc_logger_file_init(htc_logger_i *l, struct htc_file_system_i *fs, const char *file, bool append);

bool htc_logger_file_printf(htc_logger_i *l, const char *msg, ...);

bool htc_logger_file_shutdown(htc_logger_i *l);

bool htc_logger_file_init(htc_logger_i *l, htc_file_system_i *fs, const char *file, bool append) {
	htc_file_i f;

	if (l == NULL || fs == NULL) {
		return false;
	}

	f = fs->open_output(fs, file, append);

	if (f.is_valid(&f) == false) {
		return false;
	}

	l->printf = &htc_logger_file_printf;

	memcpy(&(l->inst), &f, sizeof(htc_file_i));

	return true;
}

bool htc_logger_file_printf(htc_logger_i *l, const char *msg, ...) {
	va_list args;
	char buffer[HTC_LOGGER_BUFFER_SIZE];
	htc_file_i f;
	size_t len = 0;

	if (l == NULL || msg == NULL) {
		return false;
	}

	memcpy(&f, &(l->inst), sizeof(htc_file_i));

	if (f.is_valid(&f) == false) {
		return false;
	}

	va_start(args, msg);

	if (vsprintf(buffer, msg, args) < 0) {
		return false;
	}

	len = strlen(buffer);

	if (len == 0) {
		return false;
	}

	buffer[len] = '\r';
	buffer[len + 1] = '\n';
	buffer[len + 2] = '\0';

	if (f.write(&f, buffer, strlen(buffer)) == false) {
		return false;
	}

	va_end(args);

	return true;
}

bool htc_logger_file_shutdown(htc_logger_i *l) {
	htc_file_i f;

	if (l == NULL) {
		return false;
	}

	memcpy(&f, &(l->inst), sizeof(htc_file_i));

	if (f.is_valid(&f) == false) {
		return false;
	}

	return f.close(&f);
}

/* HTTP API */
#define HTC_HTTP_BUFFER_SIZE 4096

typedef struct htc_http_request_t {
	char *body;
	size_t content_len;
	char **headers;
	size_t headers_count;
	char *host;
	char *method;
	char *path;
	char *proxy_host;
	char *proxy_port;
	char *proxy_password;
	char *proxy_user;
} htc_http_request_t;

typedef struct htc_http_response_t {
	char *data;
	size_t data_len;
} htc_http_response_t;

bool htc_http_send(htc_http_request_t *request, htc_http_response_t *response, htc_allocator_i *allocator);

bool htc_http_send(htc_http_request_t *request, htc_http_response_t *response, htc_allocator_i *allocator) {
	htc_socket_address_o address;
	char buffer[HTC_HTTP_BUFFER_SIZE];
	enum htc_socket_connect connect;
	htc_socket_hints_t hints;
	size_t i = 0;
	char *proxy_auth = NULL;
	char *proxy_credentials = NULL;
	int32_t received = 0;
	htc_socket_o s;
	size_t size = 0;

	memset(response, 0, sizeof(htc_http_response_t));

	if (!htc_socket_init()) {
		return false;
	}

	hints.address_type = HTC_SOCKET_ADDRESS_IPV4;
	hints.socket_type = HTC_SOCKET_TCP;

	if (request->proxy_host != NULL && request->proxy_port != NULL) {
		if (htc_socket_getaddrinfo(request->proxy_host, request->proxy_port, hints, &address, 1) == 0) {
			return false;
		}
	} else {
		if (htc_socket_getaddrinfo(request->host, "80", hints, &address, 1) == 0) {
			return false;
		}
	}

	s = htc_socket_create(hints);

	if (!htc_socket_is_valid(&s)) {
		return false;
	}

	connect = htc_socket_connect(&s, &address);

	if (connect != HTC_SOCKET_CONNECT_ESTABLISHED) {
		return false;
	}

	if (request->proxy_host != NULL) {
		sprintf(buffer, "%s http://%s/%s HTTP/1.1\r\n", request->method, request->host, request->path);
	} else {
		sprintf(buffer, "%s /%s HTTP/1.1\r\n", request->method, request->path);
	}

	if (htc_socket_send(&s, buffer, strlen(buffer)) != (int32_t)strlen(buffer)) {
		return false;
	}

	sprintf(buffer, "Host: %s\r\n", request->host);

	if (htc_socket_send(&s, buffer, strlen(buffer)) != (int32_t)strlen(buffer)) {
		return false;
	}

	if (request->proxy_user) {
		proxy_credentials =
		    allocator->realloc(allocator, NULL, 0, strlen(request->proxy_user) + strlen(request->proxy_password) + 1);

		sprintf(proxy_credentials, "%s:%s", request->proxy_user, request->proxy_password);

		proxy_auth = htc_base64_encode((uint8_t *)proxy_credentials, strlen(proxy_credentials), NULL);

		sprintf(buffer, "Proxy-Authorization: BASIC %s\r\n", proxy_auth);

		if (htc_socket_send(&s, buffer, strlen(buffer)) != (int32_t)strlen(buffer)) {
			allocator->realloc(allocator, proxy_credentials, strlen(proxy_credentials), 0);
			allocator->realloc(allocator, proxy_auth, strlen(proxy_auth), 0);

			return false;
		}

		allocator->realloc(allocator, proxy_credentials, strlen(proxy_credentials), 0);
		allocator->realloc(allocator, proxy_auth, strlen(proxy_auth), 0);
	}

	if (request->headers != NULL && request->headers_count > 0) {
		for (i = 0; i < request->headers_count; i++) {
			sprintf(buffer, "%s\r\n", request->headers[i]);

			if (htc_socket_send(&s, buffer, strlen(buffer)) != (int32_t)strlen(buffer)) {
				return false;
			}
		}
	}

	if (request->body != NULL && request->content_len > 0) {
		sprintf(buffer, "Content-Length: %lu\r\n", (unsigned long)request->content_len);

		if (htc_socket_send(&s, buffer, strlen(buffer)) != (int32_t)strlen(buffer)) {
			return false;
		}
	}

	sprintf(buffer, "Connection: close\r\n\r\n");

	if (htc_socket_send(&s, buffer, strlen(buffer)) != (int32_t)strlen(buffer)) {
		return false;
	}

	if (request->body != NULL && request->content_len > 0) {
		if (htc_socket_send(&s, request->body, request->content_len) != (int32_t)request->content_len) {
			return false;
		}
	}

	response->data = allocator->realloc(allocator, NULL, 0, HTC_HTTP_BUFFER_SIZE);

	size = HTC_HTTP_BUFFER_SIZE;

	while ((received = htc_socket_recv(&s, &buffer, HTC_HTTP_BUFFER_SIZE)) > 0) {
		memcpy(&(response->data[response->data_len]), buffer, received);

		response->data_len += received;

		response->data = allocator->realloc(allocator, response->data, size, response->data_len + HTC_HTTP_BUFFER_SIZE);

		size = response->data_len + HTC_HTTP_BUFFER_SIZE;
	}

	response->data = allocator->realloc(allocator, response->data, size, response->data_len + 1);

	response->data[response->data_len] = '\0';

	if (!htc_socket_close(&s)) {
		return false;
	}

	if (!htc_socket_shutdown()) {
		return false;
	}

	return true;
}

/* Plugin */
typedef struct plugin_state_t {
	htc_allocator_i *allocator;
	htc_file_system_i *file_system;
	htc_logger_i *logger;
} plugin_state_t;

bool plugin_load(plugin_state_t *state) {
	htc_allocator_i *allocator = state->allocator;
	htc_logger_i file_logger;
	htc_logger_i *logger = state->logger;
	htc_http_request_t request;
	htc_http_response_t response;

	logger->printf(logger, "Building request...");

	memset(&request, 0, sizeof(htc_http_request_t));

	request.method = "GET";

	request.host = "www.httpbin.org";
	request.path = "/ip";

	if (!htc_http_send(&request, &response, allocator)) {
		logger->printf(logger, "error: failed to send request");

		return true;
	}

	logger->printf(logger, "%s", response.data);

	htc_logger_file_init(&file_logger, state->file_system, "./bin/response.txt", false);

	file_logger.printf(&file_logger, "%s", response.data);

	htc_logger_file_shutdown(&file_logger);

	return true;
}

void htc_plugin_main(htc_plugin_t *plugin, enum htc_plugin_operation operation) {
	switch (operation) {
	case HTC_PLUGIN_OPERATION_LOAD:
		plugin->finished = plugin_load(plugin->state);
		break;
	default:
		plugin->finished = false;
		break;
	}
}

/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/
