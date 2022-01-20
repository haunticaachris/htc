/*
# htc_thread.h - v0.4.0
Licensing information can be found at the end of this file.

A simple header-only thread library.
*/

#if !defined(HTC_THREAD_H)
#define HTC_THREAD_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_THREAD_EXPORT)
#define HTC_THREAD_EXPORT extern /* API inline, static, or extern */
#endif

#if !defined(HTC_THREAD_HAVE_STDBOOL)
#define HTC_THREAD_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_THREAD_HAVE_STDDEF)
#define HTC_THREAD_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_THREAD_HAVE_STDINT)
#define HTC_THREAD_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_THREAD_USE_DOUBLE) || HTC_THREAD_USE_DOUBLE != 0
typedef float htc_thread_float_t; /* Use float by default */
#else
typedef double htc_thread_float_t; /* Use double if HTC_THREAD_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_THREAD_LANGUAGE_C)
#define HTC_THREAD_LANGUAGE_C 0
#endif

#if !defined(HTC_THREAD_LANGUAGE_CPP)
#define HTC_THREAD_LANGUAGE_CPP 0
#endif

#if !defined(HTC_THREAD_PLATFORM_ANDROID)
#define HTC_THREAD_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_THREAD_PLATFORM_LINUX)
#define HTC_THREAD_PLATFORM_LINUX 0
#endif

#if !defined(HTC_THREAD_PLATFORM_WINDOWS)
#define HTC_THREAD_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_THREAD_PLATFORM_UNSUPPORTED)
#define HTC_THREAD_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_THREAD_LANGUAGE_C
#define HTC_THREAD_LANGUAGE_C 1
#else
#undef HTC_THREAD_LANGUAGE_CPP
#define HTC_THREAD_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_THREAD_PLATFORM_ANDROID
#define HTC_THREAD_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_THREAD_PLATFORM_LINUX
#define HTC_THREAD_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_THREAD_PLATFORM_WINDOWS
#define HTC_THREAD_PLATFORM_WINDOWS 1
#else
#undef HTC_THREAD_PLATFORM_UNSUPPORTED
#define HTC_THREAD_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_THREAD_PLATFORM_ANDROID || HTC_THREAD_PLATFORM_LINUX
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
#if HTC_THREAD_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_THREAD_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_THREAD_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_THREAD_UNUSED)
#define HTC_THREAD_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* Critical Section API */
typedef struct htc_critical_section_o {
	uint32_t opaque[16];
} htc_critical_section_o;

HTC_THREAD_EXPORT bool htc_critical_section_create(htc_critical_section_o *c);

HTC_THREAD_EXPORT bool htc_critical_section_destroy(htc_critical_section_o *c);

HTC_THREAD_EXPORT bool htc_critical_section_enter(htc_critical_section_o *c);

HTC_THREAD_EXPORT bool htc_critical_section_leave(htc_critical_section_o *c);

HTC_THREAD_EXPORT bool htc_critical_section_try_enter(htc_critical_section_o *c);

/* Thread API */
typedef void htc_thread_f(void *data);

typedef struct htc_thread_o {
	uint32_t opaque[8];
} htc_thread_o;

HTC_THREAD_EXPORT htc_thread_o htc_thread_create(htc_thread_f *function, void *arg);

HTC_THREAD_EXPORT bool htc_thread_wait(htc_thread_o *t);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_THREAD_IMPLEMENTATION)
#undef HTC_THREAD_IMPLEMENTATION

#if HTC_THREAD_PLATFORM_ANDROID == 1 || HTC_THREAD_PLATFORM_LINUX == 1
#include <pthread.h> /* For pthread functions */
#elif HTC_THREAD_PLATFORM_WINDOWS == 1
#define WIN32_LEAN_AND_MEAN 1

#include <windows.h> /* Windows stuff */
#endif

/* Critical Section API */
bool htc_critical_section_create(htc_critical_section_o *c) {
	if (c == NULL) {
		return false;
	}

#if HTC_THREAD_PLATFORM_ANDROID == 1 || HTC_THREAD_PLATFORM_LINUX == 1
	if (pthread_mutex_init((pthread_mutex_t *)(c->opaque), NULL) != 0) {
		return false;
	}
#elif HTC_THREAD_PLATFORM_WINDOWS == 1
	InitializeCriticalSection((LPCRITICAL_SECTION) & (c->opaque));
#endif

	return true;
}

bool htc_critical_section_destroy(htc_critical_section_o *c) {
	if (c == NULL) {
		return false;
	}

#if HTC_THREAD_PLATFORM_ANDROID == 1 || HTC_THREAD_PLATFORM_LINUX == 1
	if (pthread_mutex_destroy((pthread_mutex_t *)(c->opaque)) != 0) {
		return false;
	}
#elif HTC_THREAD_PLATFORM_WINDOWS == 1
	DeleteCriticalSection((LPCRITICAL_SECTION) & (c->opaque));
#endif

	return true;
}

bool htc_critical_section_enter(htc_critical_section_o *c) {
	if (c == NULL) {
		return false;
	}

#if HTC_THREAD_PLATFORM_ANDROID == 1 || HTC_THREAD_PLATFORM_LINUX == 1
	if (pthread_mutex_lock((pthread_mutex_t *)(c->opaque)) != 0) {
		return false;
	}
#elif HTC_THREAD_PLATFORM_WINDOWS == 1
	EnterCriticalSection((LPCRITICAL_SECTION) & (c->opaque));
#endif

	return true;
}

bool htc_critical_section_leave(htc_critical_section_o *c) {
	if (c == NULL) {
		return false;
	}

#if HTC_THREAD_PLATFORM_ANDROID == 1 || HTC_THREAD_PLATFORM_LINUX == 1
	if (pthread_mutex_unlock((pthread_mutex_t *)(c->opaque)) == 0) {
		return false;
	}
#elif HTC_THREAD_PLATFORM_WINDOWS == 1
	LeaveCriticalSection((LPCRITICAL_SECTION) & (c->opaque));
#endif

	return true;
}

bool htc_critical_section_try_enter(htc_critical_section_o *c) {
	if (c == NULL) {
		return false;
	}

#if HTC_THREAD_PLATFORM_ANDROID == 1 || HTC_THREAD_PLATFORM_LINUX == 1
	if (pthread_mutex_trylock((pthread_mutex_t *)(c->opaque)) == 0) {
		return false;
	}
#elif HTC_THREAD_PLATFORM_WINDOWS == 1
	if (TryEnterCriticalSection((LPCRITICAL_SECTION) & (c->opaque)) == 0) {
		return false;
	}
#endif

	return true;
}

/* Thread API */
/*
TODO: I'm pretty sure using this union is undefined behavior
and needs to be fixed on all platforms.
*/
typedef union htc__thread_wrapper_t {
	void (*fptr)(void *arg);
	void *(*pthread_func)(void *arg);
} htc__thread_wrapper_t;

htc_thread_o htc_thread_create(htc_thread_f *function, void *arg) {
#if HTC_THREAD_PLATFORM_ANDROID == 1 || HTC_THREAD_PLATFORM_LINUX == 1
	htc__thread_wrapper_t func;
	htc_thread_o t = {0};

	func.fptr = function;

	if (function != NULL) {
		pthread_create((pthread_t *)&(t.opaque), NULL, func.pthread_func, arg);
	}

	return t;
#elif HTC_THREAD_PLATFORM_WINDOWS == 1
	htc_thread_o t = {0};
	HANDLE handle;

	if (function != NULL) {
		handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)function, arg, 0, NULL);

		memcpy(&(t.opaque), &handle, sizeof(void *));
	}

	return t;
#endif
}

bool htc_thread_wait(htc_thread_o *t) {
#if HTC_THREAD_PLATFORM_ANDROID == 1 || HTC_THREAD_PLATFORM_LINUX == 1
	pthread_t *thread;

	if (t == NULL) {
		return false;
	}

	thread = (pthread_t *)(t->opaque);

	if (t == NULL) {
		return false;
	}

	if (pthread_join(*thread, NULL) != 0) {
		return false;
	}

	return true;
#elif HTC_THREAD_PLATFORM_WINDOWS == 1
	HANDLE *handle = NULL;

	if (t == NULL) {
		return false;
	}

	handle = (HANDLE *)(t->opaque);

	if (WaitForSingleObject(*handle, INFINITE) == WAIT_FAILED) {
		return false;
	}

	if (CloseHandle(*handle) == 0) {
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
