/*
# htc_alloc.h - v0.7.0
Licensing information can be found at the end of this file.

A simple header-only allocator API (default malloc implementation).
*/

#if !defined(HTC_ALLOC_H)
#define HTC_ALLOC_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_ALLOC_EXPORT)
#define HTC_ALLOC_EXPORT extern /* API static or extern */
#endif

#if !defined(HTC_ALLOC_HAVE_STDBOOL)
#define HTC_ALLOC_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_ALLOC_HAVE_STDDEF)
#define HTC_ALLOC_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_ALLOC_HAVE_STDINT)
#define HTC_ALLOC_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_ALLOC_USE_DOUBLE) || HTC_ALLOC_USE_DOUBLE != 0
typedef float htc_alloc_float_t; /* Use float by default */
#else
typedef double htc_alloc_float_t; /* Use double if HTC_ALLOC_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_ALLOC_LANGUAGE_C)
#define HTC_ALLOC_LANGUAGE_C 0
#endif

#if !defined(HTC_ALLOC_LANGUAGE_CPP)
#define HTC_ALLOC_LANGUAGE_CPP 0
#endif

#if !defined(HTC_ALLOC_PLATFORM_ANDROID)
#define HTC_ALLOC_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_ALLOC_PLATFORM_LINUX)
#define HTC_ALLOC_PLATFORM_LINUX 0
#endif

#if !defined(HTC_ALLOC_PLATFORM_WINDOWS)
#define HTC_ALLOC_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_ALLOC_PLATFORM_UNSUPPORTED)
#define HTC_ALLOC_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_ALLOC_LANGUAGE_C
#define HTC_ALLOC_LANGUAGE_C 1
#else
#undef HTC_ALLOC_LANGUAGE_CPP
#define HTC_ALLOC_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_ALLOC_PLATFORM_ANDROID
#define HTC_ALLOC_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_ALLOC_PLATFORM_LINUX
#define HTC_ALLOC_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_ALLOC_PLATFORM_WINDOWS
#define HTC_ALLOC_PLATFORM_WINDOWS 1
#else
#undef HTC_ALLOC_PLATFORM_UNSUPPORTED
#define HTC_ALLOC_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_ALLOC_PLATFORM_ANDROID || HTC_ALLOC_PLATFORM_LINUX
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
#if HTC_ALLOC_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_ALLOC_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_ALLOC_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_ALLOC_UNUSED)
#define HTC_ALLOC_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* Allocator API */
typedef struct htc_allocator_o {
	uint32_t opaque[8];
} htc_allocator_o;

typedef struct htc_allocator_i {
	htc_allocator_o o;
	void *(*realloc)(struct htc_allocator_i *allocator, void *ptr, size_t old_size, size_t new_size);
} htc_allocator_i;

HTC_ALLOC_EXPORT bool htc_allocator_init(htc_allocator_i *allocator);

HTC_ALLOC_EXPORT void *htc_allocator_realloc(htc_allocator_i *allocator, void *ptr, size_t old_size, size_t new_size);

HTC_ALLOC_EXPORT bool htc_allocator_temp_init(htc_allocator_i *allocator, htc_allocator_i *instance, size_t size);

HTC_ALLOC_EXPORT void *htc_allocator_temp_realloc(htc_allocator_i *allocator, void *ptr, size_t old_size, size_t new_size);

HTC_ALLOC_EXPORT bool htc_allocator_temp_shutdown(htc_allocator_i *allocator);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_ALLOC_IMPLEMENTATION)
#undef HTC_ALLOC_IMPLEMENTATION

#if HTC_ALLOC_PLATFORM_ANDROID == 1 || HTC_ALLOC_PLATFORM_LINUX == 1
#include <stdlib.h> /* malloc / free functions */
#include <string.h> /* For memcpy */
#elif HTC_ALLOC_PLATFORM_WINDOWS == 1
#define WIN32_LEAN_AND_MEAN 1
#include <stdlib.h> /* malloc / free functions */
#include <string.h> /* For memcpy */
#endif

/* Allocator API */
bool htc_allocator_init(htc_allocator_i *allocator) {
	if (allocator == NULL) {
		return false;
	}

	allocator->realloc = &htc_allocator_realloc;

	return true;
}

void *htc_allocator_realloc(htc_allocator_i *allocator, void *ptr, size_t old_size, size_t new_size) {
	if (allocator == NULL) {
		return NULL;
	}

	if (ptr == NULL) {
		return malloc(new_size);
	} else if (new_size == 0 && old_size != 0) {
		free(ptr);

		return NULL;
	} else if (ptr != NULL && new_size != 0) {
		return realloc(ptr, new_size);
	}

	return NULL;
}

typedef struct htc__allocator_temp_o {
	htc_allocator_i *allocator;
	size_t bytes_allocated;
	uint8_t *ptr;
	size_t size;
} htc__allocator_temp_o;

bool htc_allocator_temp_init(htc_allocator_i *allocator, htc_allocator_i *instance, size_t size) {
    htc__allocator_temp_o object;

	if (allocator == NULL || instance == NULL || size == 0) {
		return false;
	}
	
	object.allocator = instance;
	object.bytes_allocated = 0;
    object.ptr = (uint8_t*)instance->realloc(instance, NULL, 0, size);
    object.size = size;
    
    if (object.ptr == NULL) {
        return false;
    }
    
    memcpy(&(allocator->o), &object, sizeof(htc__allocator_temp_o));

	allocator->realloc = &htc_allocator_temp_realloc;

	return true;
}

void *htc_allocator_temp_realloc(htc_allocator_i *allocator, void *ptr, size_t old_size, size_t new_size) {
	htc__allocator_temp_o object;
	void *ret = NULL;

	if (allocator == NULL) {
		return NULL;
	}
	
	memcpy(&object, &(allocator->o), sizeof(htc__allocator_temp_o));
	
	if (ptr == NULL && old_size == 0) {
        if (object.bytes_allocated + new_size > object.size) {
            return NULL;
        }

		ret = &(object.ptr[object.bytes_allocated]);

        object.bytes_allocated += new_size;
	} else if (new_size == 0 && old_size != 0) {
		object.bytes_allocated = 0;
	} else if (ptr != NULL && new_size != 0) {
        if (object.bytes_allocated + new_size > object.size) {
            return NULL;
        }

		ret = &(object.ptr[object.bytes_allocated]);

        if (old_size > new_size) {
		    memcpy(ret, ptr, new_size);
        } else {
            memcpy(ret, ptr, old_size);
        }

        object.bytes_allocated += new_size;
	}
	
    memcpy(&(allocator->o), &object, sizeof(htc__allocator_temp_o));

    return ret;
}

bool htc_allocator_temp_shutdown(htc_allocator_i *allocator) {
    htc__allocator_temp_o object;
	
	if (allocator == NULL) {
		return false;
	}
    		
    memcpy(&object, &(allocator->o), sizeof(htc__allocator_temp_o));
    
    object.allocator->realloc(object.allocator, object.ptr, object.size, 0);
    
    memset(allocator, 0, sizeof(htc_allocator_i));
	
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
