/*
# htc_socket.h - v0.7.0
Licensing information can be found at the end of this file.

A simple header-only networking library.
*/

#if !defined(HTC_SOCKET_H)
#define HTC_SOCKET_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_SOCKET_ADDRESS_STRLEN)
#define HTC_SOCKET_ADDRESS_STRLEN 46 /* Socket address max string length (45 + NULL) */
#endif

#if !defined(HTC_SOCKET_EXPORT)
#define HTC_SOCKET_EXPORT extern /* API inline, static, or extern */
#endif

#if !defined(HTC_SOCKET_HAVE_STDBOOL)
#define HTC_SOCKET_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_SOCKET_HAVE_STDDEF)
#define HTC_SOCKET_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_SOCKET_HAVE_STDINT)
#define HTC_SOCKET_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_SOCKET_USE_DOUBLE) || HTC_SOCKET_USE_DOUBLE != 0
typedef float htc_socket_float_t; /* Use float by default */
#else
typedef double htc_socket_float_t; /* Use double if HTC_SOCKET_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_SOCKET_LANGUAGE_C)
#define HTC_SOCKET_LANGUAGE_C 0
#endif

#if !defined(HTC_SOCKET_LANGUAGE_CPP)
#define HTC_SOCKET_LANGUAGE_CPP 0
#endif

#if !defined(HTC_SOCKET_PLATFORM_ANDROID)
#define HTC_SOCKET_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_SOCKET_PLATFORM_LINUX)
#define HTC_SOCKET_PLATFORM_LINUX 0
#endif

#if !defined(HTC_SOCKET_PLATFORM_WINDOWS)
#define HTC_SOCKET_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_SOCKET_PLATFORM_UNSUPPORTED)
#define HTC_SOCKET_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_SOCKET_LANGUAGE_C
#define HTC_SOCKET_LANGUAGE_C 1
#else
#undef HTC_SOCKET_LANGUAGE_CPP
#define HTC_SOCKET_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_SOCKET_PLATFORM_ANDROID
#define HTC_SOCKET_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_SOCKET_PLATFORM_LINUX
#define HTC_SOCKET_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_SOCKET_PLATFORM_WINDOWS
#define HTC_SOCKET_PLATFORM_WINDOWS 1
#else
#undef HTC_SOCKET_PLATFORM_UNSUPPORTED
#define HTC_SOCKET_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_SOCKET_PLATFORM_ANDROID || HTC_SOCKET_PLATFORM_LINUX
#if !defined(_BSD_SOURCE)
#define _BSD_SOURCE 1
#endif

#if !defined(_DEFAULT_SOURCE)
#define _DEFAULT_SOURCE 1
#endif

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif

#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 600
#endif
#endif

/*
Must be included after certain platform defines
in order to work correctly so we add these last.
*/
#if HTC_SOCKET_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_SOCKET_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_SOCKET_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_SOCKET_UNUSED)
#define HTC_SOCKET_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* Socket API */
enum htc_socket_connect { HTC_SOCKET_CONNECT_ESTABLISHED, HTC_SOCKET_CONNECT_FAILED, HTC_SOCKET_CONNECT_PENDING };

enum htc_socket { HTC_SOCKET_TCP, HTC_SOCKET_UDP, HTC_SOCKET_INVALID };

enum htc_socket_address { HTC_SOCKET_ADDRESS_IPV4, HTC_SOCKET_ADDRESS_IPV6, HTC_SOCKET_ADDRESS_UNSPEC };

enum htc_socket_option { HTC_SOCKET_OPTION_NONBLOCK, HTC_SOCKET_OPTION_REUSEADDR };

typedef struct htc_socket_allocator_o {
	uint32_t opaque[8];
} htc_socket_allocator_o;

typedef struct htc_socket_allocator_i {
	htc_socket_allocator_o o;
	void *(*realloc)(struct htc_socket_allocator_i *allocator, void *ptr, size_t old_size, size_t new_size);
} htc_socket_allocator_i;

typedef struct htc_socket_o {
	uint32_t opaque[4];
} htc_socket_o;

typedef struct htc_socket_address_o {
	uint32_t opaque[16];
} htc_socket_address_o;

typedef struct htc_socket_hints_t {
	enum htc_socket_address address_type;
	enum htc_socket socket_type;
} htc_socket_hints_t;

HTC_SOCKET_EXPORT htc_socket_o htc_socket_accept(htc_socket_o *s, htc_socket_address_o *address);

HTC_SOCKET_EXPORT bool htc_socket_bind(htc_socket_o *s, htc_socket_address_o *address);

HTC_SOCKET_EXPORT bool htc_socket_close(htc_socket_o *s);

HTC_SOCKET_EXPORT enum htc_socket_connect htc_socket_connect(htc_socket_o *s, htc_socket_address_o *address);

HTC_SOCKET_EXPORT htc_socket_o htc_socket_create(htc_socket_hints_t hints);

HTC_SOCKET_EXPORT uint32_t htc_socket_getaddrinfo(const char *host, const char *service, htc_socket_hints_t hints,
                                                  htc_socket_address_o *addresses, uint32_t size);

HTC_SOCKET_EXPORT bool htc_socket_init(void);

HTC_SOCKET_EXPORT bool htc_socket_is_valid(struct htc_socket_o *s);

HTC_SOCKET_EXPORT bool htc_socket_listen(htc_socket_o *s, uint32_t queue);

HTC_SOCKET_EXPORT const char *htc_socket_ntop(htc_socket_address_o *address, htc_socket_allocator_i *allocator);

HTC_SOCKET_EXPORT int32_t htc_socket_recv(htc_socket_o *s, void *buffer, size_t bytes);

HTC_SOCKET_EXPORT int32_t htc_socket_recvfrom(htc_socket_o *s, void *buffer, size_t bytes,
                                              htc_socket_address_o *address);

HTC_SOCKET_EXPORT int32_t htc_socket_send(htc_socket_o *s, const void *buffer, size_t bytes);

HTC_SOCKET_EXPORT int32_t htc_socket_sendto(htc_socket_o *s, const void *buffer, size_t bytes,
                                            htc_socket_address_o *address);

HTC_SOCKET_EXPORT bool htc_socket_set_option(htc_socket_o *s, enum htc_socket_option option, bool enabled);

HTC_SOCKET_EXPORT bool htc_socket_shutdown(void);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_SOCKET_IMPLEMENTATION)
#undef HTC_SOCKET_IMPLEMENTATION

#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
#include <arpa/inet.h>  /* For sockets */
#include <errno.h>      /* For errno */
#include <fcntl.h>      /* fcntl for non blocking sockets */
#include <netdb.h>      /* For sockets */
#include <netinet/in.h> /* For sockets */
#include <stdio.h>      /* For io functions */
#include <string.h>     /* For memset */
#include <sys/socket.h> /* For sockets */
#include <sys/types.h>  /* For sockets */
#include <unistd.h>     /* For close */
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
#define WIN32_LEAN_AND_MEAN 1
#pragma comment(lib, "Ws2_32.lib") /* Winsock */

#include <stdio.h>      /* For io functions */
#include <string.h> /* For memset */

#include <windows.h>  /* Windows stuff */
#include <winsock2.h> /* Sockets */
#include <ws2tcpip.h> /* Sockets */
#endif

/* Socket API */
htc_socket_o htc_socket_accept(htc_socket_o *s, htc_socket_address_o *address) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;
	socklen_t len = sizeof(struct sockaddr_storage);
	htc_socket_o socket;
	struct sockaddr_storage storage;

	address = NULL;

	memset(&socket, 0, sizeof(htc_socket_o));

	memcpy(socket.opaque, &handle, sizeof(int));

	memcpy(&handle, &(s->opaque), sizeof(int));

	if (!htc_socket_is_valid(s)) {
		return socket;
	}

	handle = accept(handle, (struct sockaddr *)&storage, &len);

	if (handle == -1) {
		return socket;
	}

	memcpy(address->opaque, &storage, len);

	memcpy(&(socket.opaque), &handle, sizeof(int));

	return socket;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;
	socklen_t len = sizeof(struct sockaddr_storage);
	htc_socket_o socket;
	struct sockaddr_storage storage;

	address = NULL;

	memset(&socket, 0, sizeof(htc_socket_o));

	memcpy(socket.opaque, &handle, sizeof(SOCKET));

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	if (!htc_socket_is_valid(s)) {
		return socket;
	}

	handle = accept(handle, (struct sockaddr *)&storage, &len);

	if (handle == INVALID_SOCKET) {
		return socket;
	}

	memcpy(address->opaque, &storage, len);

	memcpy(&(socket.opaque), &handle, sizeof(SOCKET));

	return socket;
#endif
}

bool htc_socket_bind(htc_socket_o *s, htc_socket_address_o *address) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;
	struct addrinfo *servinfo;

	if (!htc_socket_is_valid(s)) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	servinfo = (struct addrinfo *)address->opaque;

	if (bind(handle, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		return false;
	}

	return true;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;
	struct addrinfo *servinfo;

	if (!htc_socket_is_valid(s)) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	servinfo = (struct addrinfo *)address->opaque;

	if (bind(handle, servinfo->ai_addr, servinfo->ai_addrlen) == SOCKET_ERROR) {
		return false;
	}

	return true;
#endif
}

bool htc_socket_close(struct htc_socket_o *s) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;

	if (!htc_socket_is_valid(s)) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	if (close(handle) == -1) {
		return false;
	}

	return true;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;

	if (!htc_socket_is_valid(s)) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	if (closesocket(handle) == SOCKET_ERROR) {
		return false;
	}

	return true;
#endif
}

enum htc_socket_connect htc_socket_connect(htc_socket_o *s, htc_socket_address_o *address) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;
	struct addrinfo *servinfo;

	if (!htc_socket_is_valid(s)) {
		return HTC_SOCKET_CONNECT_FAILED;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	servinfo = (struct addrinfo *)address;

	if (connect(handle, servinfo->ai_addr, servinfo->ai_addrlen) != -1) {
		return HTC_SOCKET_CONNECT_ESTABLISHED;
	}

	if (errno == EISCONN) {
		return HTC_SOCKET_CONNECT_ESTABLISHED;
	}

	if (errno == EAGAIN || errno == EALREADY || errno == EINPROGRESS) {
		return HTC_SOCKET_CONNECT_PENDING;
	}

	return HTC_SOCKET_CONNECT_FAILED;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;
	struct addrinfo *servinfo;
	int err = 0;

	if (!htc_socket_is_valid(s)) {
		return HTC_SOCKET_CONNECT_FAILED;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	servinfo = (struct addrinfo *)address;

	if (connect(handle, servinfo->ai_addr, servinfo->ai_addrlen) != SOCKET_ERROR) {
		return HTC_SOCKET_CONNECT_ESTABLISHED;
	}

	err = WSAGetLastError();

	if (err == WSAEISCONN) {
		return HTC_SOCKET_CONNECT_ESTABLISHED;
	}

	if (err == WSAEALREADY || err == WSAEINVAL || err == WSAEWOULDBLOCK) {
		return HTC_SOCKET_CONNECT_PENDING;
	}

	return HTC_SOCKET_CONNECT_FAILED;
#endif
}

htc_socket_o htc_socket_create(htc_socket_hints_t hints) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	htc_socket_o sock = {0};
	int handle = -1;

	switch (hints.socket_type) {
	case HTC_SOCKET_TCP:
		if (hints.address_type == HTC_SOCKET_ADDRESS_IPV6) {
			handle = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
		} else {
			handle = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		}

		break;
	case HTC_SOCKET_UDP:
		if (hints.address_type == HTC_SOCKET_ADDRESS_IPV6) {
			handle = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
		} else {
			handle = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		}

		break;
	default:
		handle = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		break;
	}

	memcpy(&(sock.opaque), &handle, sizeof(int));

	return sock;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	htc_socket_o sock = {0};
	SOCKET handle = INVALID_SOCKET;

	switch (hints.socket_type) {
	case HTC_SOCKET_TCP:
		if (hints.address_type == HTC_SOCKET_ADDRESS_IPV6) {
			handle = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
		} else {
			handle = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		}

		break;
	case HTC_SOCKET_UDP:
		if (hints.address_type == HTC_SOCKET_ADDRESS_IPV6) {
			handle = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
		} else {
			handle = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		}

		break;
	default:
		handle = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		break;
	}

	memcpy(&(sock.opaque), &handle, sizeof(SOCKET));

	return sock;
#endif
}

uint32_t htc_socket_getaddrinfo(const char *host, const char *service, htc_socket_hints_t hints,
                                htc_socket_address_o *addresses, uint32_t size) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	uint32_t count = 0;
	struct addrinfo h;
	struct addrinfo *servinfo = NULL;

	memset(&h, 0, sizeof(struct addrinfo));
	memset(addresses, 0, (sizeof(htc_socket_address_o) * size));

	switch (hints.address_type) {
	case HTC_SOCKET_ADDRESS_IPV4:
		h.ai_family = AF_INET;
		break;
	case HTC_SOCKET_ADDRESS_IPV6:
		h.ai_family = AF_INET6;
		break;
	default:
		h.ai_family = AF_UNSPEC;
		break;
	}

	switch (hints.socket_type) {
	case HTC_SOCKET_UDP:
		h.ai_socktype = SOCK_DGRAM;
		break;
	default:
		h.ai_socktype = SOCK_STREAM;
		break;
	}

	if (getaddrinfo(host, service, &h, &servinfo) != 0) {
		return 0;
	}

	for (count = 0; count < size; count++) {
		if (servinfo != NULL) {
			memcpy(&(addresses[count]), servinfo, sizeof(htc_socket_address_o));

			servinfo = servinfo->ai_next;

			continue;
		}

		break;
	}

	return count;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	uint32_t count = 0;
	struct addrinfo h;
	struct addrinfo *servinfo = NULL;

	memset(&h, 0, sizeof(struct addrinfo));
	memset(addresses, 0, (sizeof(htc_socket_address_o) * size));

	switch (hints.address_type) {
	case HTC_SOCKET_ADDRESS_IPV4:
		h.ai_family = AF_INET;
		break;
	case HTC_SOCKET_ADDRESS_IPV6:
		h.ai_family = AF_INET6;
		break;
	default:
		h.ai_family = AF_UNSPEC;
		break;
	}

	switch (hints.socket_type) {
	case HTC_SOCKET_UDP:
		h.ai_socktype = SOCK_DGRAM;
		break;
	default:
		h.ai_socktype = SOCK_STREAM;
		break;
	}

	if (getaddrinfo(host, service, &h, &servinfo) != 0) {
		return 0;
	}

	for (count = 0; count < size; count++) {
		if (servinfo != NULL) {
			memcpy(&(addresses[count]), servinfo, sizeof(htc_socket_address_o));

			servinfo = servinfo->ai_next;

			continue;
		}

		break;
	}

	return count;
#endif
}

bool htc_socket_init(void) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	return true;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return false;
	}

	return true;
#endif
}

bool htc_socket_is_valid(struct htc_socket_o *s) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;

	if (s == NULL) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	if (handle != -1) {
		return true;
	}

	return false;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;

	if (s == NULL) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	if (handle != INVALID_SOCKET) {
		return true;
	}

	return false;
#endif
}

bool htc_socket_listen(htc_socket_o *s, uint32_t queue) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;

	if (!htc_socket_is_valid(s)) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	if (listen(handle, (int)queue) == -1) {
		return false;
	}

	return true;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;

	if (!htc_socket_is_valid(s)) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	if (queue > 4096) {
		return false;
	}

	if (listen(handle, (int)queue) == SOCKET_ERROR) {
		return false;
	}

	return true;
#endif
}

const char *htc_socket_ntop(htc_socket_address_o *address, htc_socket_allocator_i *allocator) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	struct sockaddr *addr;
	char *buffer = (char *)allocator->realloc(allocator, NULL, 0, HTC_SOCKET_ADDRESS_STRLEN);
	struct addrinfo *servinfo;

	if (address == NULL || allocator == NULL || buffer == NULL) {
		return NULL;
	}

	servinfo = (struct addrinfo *)address->opaque;
	addr = servinfo->ai_addr;

	switch (addr->sa_family) {
	case AF_INET:
		if (inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), (char *)buffer, HTC_SOCKET_ADDRESS_STRLEN) !=
		    NULL) {
			return buffer;
		}

		break;

	case AF_INET6:
		if (inet_ntop(AF_INET, &(((struct sockaddr_in6 *)addr)->sin6_addr), (char *)buffer, HTC_SOCKET_ADDRESS_STRLEN) !=
		    NULL) {
			return buffer;
		}

		break;

	default:
		return NULL;
	}

	return buffer;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	struct sockaddr *addr;
	char *buffer = (char *)htc_allocator_alloc((*a), HTC_SOCKET_ADDRESS_STRLEN);
	struct addrinfo *servinfo;

	if (address == NULL || a == NULL || buffer == NULL) {
		return NULL;
	}

	servinfo = (struct addrinfo *)address->opaque;
	addr = servinfo->ai_addr;

	switch (addr->sa_family) {
	case AF_INET:
		if (inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), (char *)buffer, HTC_SOCKET_ADDRESS_STRLEN) !=
		    NULL) {
			return buffer;
		}

		break;

	case AF_INET6:
		if (inet_ntop(AF_INET, &(((struct sockaddr_in6 *)addr)->sin6_addr), (char *)buffer, HTC_SOCKET_ADDRESS_STRLEN) !=
		    NULL) {
			return buffer;
		}

		break;

	default:
		return NULL;
	}

	return buffer;
#endif
}

int32_t htc_socket_recv(htc_socket_o *s, void *buffer, size_t bytes) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;

	if (!htc_socket_is_valid(s)) {
		return -1;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	return (int32_t)recv(handle, buffer, bytes, 0);
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;

	if (!htc_socket_is_valid(s)) {
		return -1;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	return (int32_t)recv(handle, (char *)buffer, (int)bytes, 0);
#endif
}

int32_t htc_socket_recvfrom(htc_socket_o *s, void *buffer, size_t bytes, htc_socket_address_o *address) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;
	socklen_t len = sizeof(struct sockaddr_storage);

	if (!htc_socket_is_valid(s)) {
		return -1;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	return (int32_t)recvfrom(handle, buffer, bytes, 0, ((struct addrinfo *)address)->ai_addr, &len);
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;
	int len = sizeof(struct sockaddr_storage);

	if (!htc_socket_is_valid(s)) {
		return -1;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	return (int32_t)recvfrom(handle, (char *)buffer, (int)bytes, 0, ((struct addrinfo *)address)->ai_addr, &len);
#endif
}

int32_t htc_socket_send(htc_socket_o *s, const void *buffer, size_t bytes) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;

	if (!htc_socket_is_valid(s)) {
		return -1;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	return (int32_t)send(handle, buffer, bytes, 0);
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;

	if (!htc_socket_is_valid(s)) {
		return -1;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	return (int32_t)send(handle, (char *)buffer, (int)bytes, 0);
#endif
}

int32_t htc_socket_sendto(htc_socket_o *s, const void *buffer, size_t bytes, htc_socket_address_o *address) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int handle = -1;

	if (!htc_socket_is_valid(s)) {
		return -1;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	return (int32_t)sendto(handle, buffer, bytes, 0, ((struct addrinfo *)address)->ai_addr,
	                       sizeof(struct sockaddr_storage));
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	SOCKET handle = INVALID_SOCKET;

	if (!htc_socket_is_valid(s)) {
		return -1;
	}

	memcpy(&handle, &(s->opaque), sizeof(SOCKET));

	return (int32_t)sendto(handle, (char *)buffer, (int)bytes, 0, ((struct addrinfo *)address)->ai_addr,
	                       sizeof(struct sockaddr_storage));
#endif
}

bool htc_socket_set_option(htc_socket_o *s, enum htc_socket_option option, bool enabled) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	int flags = 0;
	int handle = -1;
	int len = 0;
	int result = 0;

	if (!htc_socket_is_valid(s)) {
		return false;
	}

	memcpy(&handle, &(s->opaque), sizeof(int));

	switch (option) {
	case HTC_SOCKET_OPTION_NONBLOCK:
		flags = fcntl(handle, F_GETFL, 0);

		if (flags == -1) {
			return false;
		}

		if (enabled == true) {
			flags |= O_NONBLOCK;
		} else {
			flags &= ~O_NONBLOCK;
		}

		if (fcntl(handle, F_SETFL, flags) != -1) {
			return true;
		}

		break;
	case HTC_SOCKET_OPTION_REUSEADDR:
		flags = 1;
		len = sizeof(flags);

		result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (char *)&flags, len);

		if (result == 0) {
			return true;
		}

		break;
	}

	return false;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	BOOL b = FALSE;
	SOCKET handle = INVALID_SOCKET;
	int len = 0;
	u_long mode = 0;

	if (htc_socket_is_valid(s)) {
		memcpy(&handle, &(s->opaque), sizeof(SOCKET));

		switch (option) {
		case HTC_SOCKET_OPTION_NONBLOCK:
			if (enabled == true) {
				mode = 1;
			} else {
				mode = 0;
			}

			if (ioctlsocket(handle, FIONBIO, &mode) != SOCKET_ERROR) {
				return true;
			}

			break;
		case HTC_SOCKET_OPTION_REUSEADDR:
			b = TRUE;
			len = sizeof(b);

			result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (char *)&b, len);

			if (result != SOCKET_ERROR) {
				return true;
			}

			break;
		}
	}

	return false;
#endif
}

bool htc_socket_shutdown(void) {
#if HTC_SOCKET_PLATFORM_ANDROID == 1 || HTC_SOCKET_PLATFORM_LINUX == 1
	return true;
#elif HTC_SOCKET_PLATFORM_WINDOWS == 1
	if (WSACleanup() == SOCKET_ERROR) {
		return false;
	}

	return true;
#endif
}

#endif

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
