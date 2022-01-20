/*
# htc_plugin.h - v0.7.0
Licensing information can be found at the end of this file.

A simple header-only plugin library with support for live reload
of code.
*/

#if !defined(HTC_PLUGIN_H)
#define HTC_PLUGIN_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_PLUGIN_EXPORT)
#define HTC_PLUGIN_EXPORT extern /* API inline, static, or extern */
#endif

#if !defined(HTC_PLUGIN_ENTRY)
#define HTC_PLUGIN_ENTRY "htc_plugin_main" /* Default plugin entry point */
#endif

#if !defined(HTC_PLUGIN_HAVE_STDBOOL)
#define HTC_PLUGIN_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_PLUGIN_HAVE_STDDEF)
#define HTC_PLUGIN_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_PLUGIN_HAVE_STDINT)
#define HTC_PLUGIN_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_PLUGIN_STATIC)
#define HTC_PLUGIN_STATIC 0 /* Set to 1 for static plugin linking */
#endif

#if !defined(HTC_PLUGIN_USE_DOUBLE) || HTC_PLUGIN_USE_DOUBLE != 0
typedef float htc_plugin_float_t; /* Use float by default */
#else
typedef double htc_plugin_float_t; /* Use double if HTC_PLUGIN_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_PLUGIN_LANGUAGE_C)
#define HTC_PLUGIN_LANGUAGE_C 0
#endif

#if !defined(HTC_PLUGIN_LANGUAGE_CPP)
#define HTC_PLUGIN_LANGUAGE_CPP 0
#endif

#if !defined(HTC_PLUGIN_PLATFORM_ANDROID)
#define HTC_PLUGIN_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_PLUGIN_PLATFORM_LINUX)
#define HTC_PLUGIN_PLATFORM_LINUX 0
#endif

#if !defined(HTC_PLUGIN_PLATFORM_WINDOWS)
#define HTC_PLUGIN_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_PLUGIN_PLATFORM_UNSUPPORTED)
#define HTC_PLUGIN_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_PLUGIN_LANGUAGE_C
#define HTC_PLUGIN_LANGUAGE_C 1
#else
#undef HTC_PLUGIN_LANGUAGE_CPP
#define HTC_PLUGIN_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_PLUGIN_PLATFORM_ANDROID
#define HTC_PLUGIN_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_PLUGIN_PLATFORM_LINUX
#define HTC_PLUGIN_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_PLUGIN_PLATFORM_WINDOWS
#define HTC_PLUGIN_PLATFORM_WINDOWS 1
#else
#undef HTC_PLUGIN_PLATFORM_UNSUPPORTED
#define HTC_PLUGIN_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_PLUGIN_PLATFORM_ANDROID || HTC_PLUGIN_PLATFORM_LINUX
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
#if HTC_PLUGIN_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_PLUGIN_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_PLUGIN_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_PLUGIN_UNUSED)
#define HTC_PLUGIN_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* Lib API */
typedef struct htc_lib_o {
	uint32_t opaque[4];
} htc_lib_o;

typedef union htc_lib_symbol_t {
	void *ptr;
	void (*fptr)(void);
} htc_lib_symbol_t;

HTC_PLUGIN_EXPORT bool htc_lib_close(htc_lib_o *l);

HTC_PLUGIN_EXPORT bool htc_lib_is_valid(struct htc_lib_o *l);

HTC_PLUGIN_EXPORT htc_lib_o htc_lib_open(const char *path);

HTC_PLUGIN_EXPORT htc_lib_symbol_t htc_lib_symbol(htc_lib_o *l, const char *symbol);

/* Plugin API */
typedef struct htc_plugin_t htc_plugin_t;

enum htc_plugin_operation {
	HTC_PLUGIN_OPERATION_CLOSE,
	HTC_PLUGIN_OPERATION_LOAD,
	HTC_PLUGIN_OPERATION_UNLOAD,
	HTC_PLUGIN_OPERATION_UPDATE
};

typedef void (*htc_plugin_main_f)(htc_plugin_t *plugin, enum htc_plugin_operation operation);

struct htc_plugin_t {
	bool finished;
	htc_plugin_main_f func;
	htc_lib_o lib;
	char path[HTC_FILE_PATH_STRLEN];
	void *state;
	size_t version;
};

/* Default plugin entry point */
extern void htc_plugin_main(htc_plugin_t *plugin, enum htc_plugin_operation operation);

HTC_PLUGIN_EXPORT bool htc_plugin_close(htc_plugin_t *plugin);

HTC_PLUGIN_EXPORT bool htc_plugin_open(htc_plugin_t *plugin, const char *path);

HTC_PLUGIN_EXPORT bool htc_plugin_update(htc_plugin_t *plugin, bool reload);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_PLUGIN_IMPLEMENTATION)
#undef HTC_PLUGIN_IMPLEMENTATION

#if HTC_PLUGIN_PLATFORM_ANDROID == 1 || HTC_PLUGIN_PLATFORM_LINUX == 1
#include <dlfcn.h> /* Linux functions to load dynamic libraries */
#elif HTC_PLUGIN_PLATFORM_WINDOWS == 1
#define WIN32_LEAN_AND_MEAN 1

#include <windows.h> /* Windows stuff */
#endif

/* Lib API */
bool htc_lib_close(htc_lib_o *l) {
	void *lib = NULL;

	if (!htc_lib_is_valid(l)) {
		return false;
	}

	memcpy(&lib, &(l->opaque), sizeof(void *));

#if HTC_PLUGIN_PLATFORM_ANDROID == 1 || HTC_PLUGIN_PLATFORM_LINUX == 1
	if (dlclose(lib) != 0) {
		return false;
	}
#elif HTC_PLUGIN_PLATFORM_WINDOWS == 1
	if (FreeLibrary((HMODULE)lib) == 0) {
		return false;
	}
#endif

	memset(l, 0, sizeof(htc_lib_o));

	return true;
}

bool htc_lib_is_valid(struct htc_lib_o *l) {
#if HTC_PLUGIN_PLATFORM_ANDROID == 1 || HTC_PLUGIN_PLATFORM_LINUX == 1 || HTC_PLUGIN_PLATFORM_WINDOWS == 1
	void *lib;

	memcpy(&lib, &(l->opaque), sizeof(void *));

	if (lib != NULL) {
		return true;
	}
#endif

	return false;
}

htc_lib_o htc_lib_open(const char *path) {
#if HTC_PLUGIN_PLATFORM_ANDROID == 1 || HTC_PLUGIN_PLATFORM_LINUX == 1
	htc_lib_o l;
	void *lib = NULL;

	memset(&l, 0, sizeof(htc_lib_o));

	lib = dlopen(path, RTLD_NOW);

	if (lib != NULL) {
		memcpy(&(l.opaque), &lib, sizeof(void *));
	}

	return l;
#elif HTC_PLUGIN_PLATFORM_WINDOWS == 1
	htc_lib_o l;
	void *lib = NULL;

	memset(&l, 0, sizeof(htc_lib_o));

	lib = LoadLibrary(path);

	if (lib != NULL) {
		memcpy(&(l.opaque), &lib, sizeof(void *));
	}

	return l;
#endif
}

htc_lib_symbol_t htc_lib_symbol(htc_lib_o *l, const char *symbol) {
	void *lib = NULL;
	htc_lib_symbol_t s = {NULL};

	if (htc_lib_is_valid(l)) {
		memcpy(&lib, &(l->opaque), sizeof(void *));

#if HTC_PLUGIN_PLATFORM_ANDROID == 1 || HTC_PLUGIN_PLATFORM_LINUX == 1
		s.ptr = dlsym(lib, symbol);
#elif HTC_PLUGIN_PLATFORM_WINDOWS == 1
		s.ptr = GetProcAddress((HMODULE)lib, symbol);
#endif
	}

	return s;
}

/* Plugin API */
bool htc_plugin_close(htc_plugin_t *plugin) {
	htc_lib_o lib = {0};

	if (plugin == NULL) {
		return false;
	}

	plugin->finished = true;

	if (plugin->func == NULL) {
		return false;
	}

	plugin->func(plugin, HTC_PLUGIN_OPERATION_CLOSE);

	plugin->func = NULL;

	lib = plugin->lib;

	if (!htc_lib_close(&lib) && HTC_PLUGIN_STATIC == 0) {
		return false;
	}

	return true;
}

/*
When static linking you must assign the entry function
to the htc_plugin_t struct.
*/
bool htc_plugin_open(htc_plugin_t *plugin, const char *path) {
	htc_lib_o lib = {0};
	htc_lib_symbol_t symbol = {0};

	if (plugin == NULL) {
		return false;
	}

	if (HTC_PLUGIN_STATIC != 0) {
		plugin->finished = false;
		plugin->version += 1;

		plugin->func(plugin, HTC_PLUGIN_OPERATION_LOAD);

		return true;
	}

	lib = htc_lib_open(path);

	if (!htc_lib_is_valid(&lib)) {
		plugin->finished = true;

		return false;
	}

	symbol = htc_lib_symbol(&lib, HTC_PLUGIN_ENTRY);

	if (symbol.fptr == NULL) {
		plugin->finished = true;

		return false;
	}

	plugin->func = (htc_plugin_main_f)symbol.fptr;
	plugin->lib = lib;
	plugin->finished = false;
	plugin->version += 1;

	memcpy(&(plugin->path), path, strlen(path) + 1);

	plugin->func(plugin, HTC_PLUGIN_OPERATION_LOAD);

	return true;
}

bool htc_plugin_update(htc_plugin_t *plugin, bool reload) {
	htc_lib_o lib = {0};

	if (plugin == NULL) {
		plugin->finished = true;

		return false;
	}

	if (plugin->func == NULL) {
		plugin->finished = true;

		return false;
	}

	if (HTC_PLUGIN_STATIC == 0) {
		if (reload) {
			plugin->func(plugin, HTC_PLUGIN_OPERATION_UNLOAD);

			lib = plugin->lib;

			if (!htc_plugin_open(plugin, plugin->path)) {
				plugin->finished = true;

				return false;
			}

			if (!htc_lib_close(&lib)) {
				return false;
			}
		}
	}

	plugin->func(plugin, HTC_PLUGIN_OPERATION_UPDATE);

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
