/*
# htc_logger.h - v0.7.0
Licensing information can be found at the end of this file.

A simple header-only logging library with a simple API.
*/

#if !defined(HTC_LOGGER_H)
#define HTC_LOGGER_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_LOGGER_EXPORT)
#define HTC_LOGGER_EXPORT extern /* API static or extern */
#endif

#if !defined(HTC_LOGGER_HAVE_STDBOOL)
#define HTC_LOGGER_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_LOGGER_HAVE_STDDEF)
#define HTC_LOGGER_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_LOGGER_HAVE_STDINT)
#define HTC_LOGGER_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_LOGGER_BUFFER_SIZE)
#define HTC_LOGGER_BUFFER_SIZE 4096 /* The default size of the internal log buffer */
#endif

#if !defined(HTC_LOGGER_USE_DOUBLE) || HTC_LOGGER_USE_DOUBLE != 0
typedef float htc_logger_float_t; /* Use float by default */
#else
typedef double htc_logger_float_t; /* Use double if HTC_LOGGER_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_LOGGER_LANGUAGE_C)
#define HTC_LOGGER_LANGUAGE_C 0
#endif

#if !defined(HTC_LOGGER_LANGUAGE_CPP)
#define HTC_LOGGER_LANGUAGE_CPP 0
#endif

#if !defined(HTC_LOGGER_PLATFORM_ANDROID)
#define HTC_LOGGER_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_LOGGER_PLATFORM_LINUX)
#define HTC_LOGGER_PLATFORM_LINUX 0
#endif

#if !defined(HTC_LOGGER_PLATFORM_WINDOWS)
#define HTC_LOGGER_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_LOGGER_PLATFORM_UNSUPPORTED)
#define HTC_LOGGER_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_LOGGER_LANGUAGE_C
#define HTC_LOGGER_LANGUAGE_C 1
#else
#undef HTC_LOGGER_LANGUAGE_CPP
#define HTC_LOGGER_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_LOGGER_PLATFORM_ANDROID
#define HTC_LOGGER_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_LOGGER_PLATFORM_LINUX
#define HTC_LOGGER_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_LOGGER_PLATFORM_WINDOWS
#define HTC_LOGGER_PLATFORM_WINDOWS 1
#else
#undef HTC_LOGGER_PLATFORM_UNSUPPORTED
#define HTC_LOGGER_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_LOGGER_PLATFORM_ANDROID || HTC_LOGGER_PLATFORM_LINUX
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
#if HTC_LOGGER_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_LOGGER_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_LOGGER_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_LOGGER_UNUSED)
#define HTC_LOGGER_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* Logging API */
typedef struct htc_logger_o {
	uint32_t opaque[16];
} htc_logger_o;

typedef struct htc_logger_i {
	htc_logger_o inst;
	bool (*printf)(struct htc_logger_i *l, const char *msg, ...);
} htc_logger_i;

HTC_LOGGER_EXPORT bool htc_logger_init(htc_logger_i *l);

HTC_LOGGER_EXPORT bool htc_logger_printf(htc_logger_i *l, const char *msg, ...);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_LOGGER_IMPLEMENTATION)
#undef HTC_LOGGER_IMPLEMENTATION

#if HTC_LOGGER_PLATFORM_ANDROID == 1 || HTC_LOGGER_PLATFORM_LINUX == 1
#include <stdarg.h> /* Var args */
#include <stdio.h>  /* For io functions */
#elif HTC_LOGGER_PLATFORM_WINDOWS == 1
#define WIN32_LEAN_AND_MEAN 1

#include <stdarg.h>  /* Var args */
#include <stdio.h>   /* For io functions */
#include <windows.h> /* Windows stuff */
#endif

/* Logging API */
bool htc_logger_init(htc_logger_i *l) {
	if (l == NULL) {
		return false;
	}

	l->printf = &htc_logger_printf;

	return true;
}

bool htc_logger_printf(htc_logger_i *l, const char *msg, ...) {
	va_list args;
	char buffer[HTC_LOGGER_BUFFER_SIZE];

	if (l == NULL || msg == NULL) {
		return false;
	}

	va_start(args, msg);

	if (vsprintf(buffer, msg, args) < 0) {
		return false;
	}

	if (printf("%s\n", buffer) < 0) {
		return false;
	}

#if HTC_LOGGER_PLATFORM_WINDOWS == 1
	OutputDebugString(buffer);
#endif

	va_end(args);

	return true;
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
