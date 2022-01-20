## HTC
Several single-file, cross-platform, public domain libraries for C/C++ that I use for learning / testing (Not meant for production code).

This is used as a base library for random projects and was not meant to be used for anything serious.

This is intended to be used as a base and extended to your needs. You will most likely want to wrap functions in a higher level API and many functions include interfaces that use function pointers to allow for multiple custom implementations (an example can be seen with the logger that includes a console and a file implementation in [examples/http](examples/http)). Every library is seperate so you only need to include what you use.
|Library|Description|
|:-|:-|
|[htc_alloc.h](include/htc_alloc.h) | A simple header-only allocator API (default malloc implementation). |
|[htc_crypto.h](include/htc_crypto.h) | A simple header-only crypto library (Used for testing not intended for production use). |
|[htc_file.h](include/htc_file.h) | A simple header-only file library with file system support. |
|[htc_logger.h](include/htc_logger.h) | A simple header-only logging library with a simple API. |
|[htc_math.h](include/htc_math.h) | A simple header-only math library for games. |
|[htc_plugin.h](include/htc_plugin.h) | A simple header-only plugin library with support for live reload of code. |
|[htc_socket.h](include/htc_socket.h) | A simple header-only networking library. |
|[htc_thread.h](include/htc_thread.h) | A simple header-only thread library. |
|[htc_time.h](include/htc_time.h) | A simple header-only date and time library. |
* Currently working on  Android / Linux / Windows (Windows APIs are not up to date and will not be fixed until the API is more stable).
* Android support is in [Termux](https://github.com/termux/termux-app) only I am working on porting to the NDK currently.
* Much of the code may be broke / buggy currently as I am doing some major refactoring.
## Building
To use this library define HTC_NAME_IMPLEMENTATION in one source file (before you include "htc_*.h") and include the headers wherever the API is required. If you use the HTC_NAME_HAVE_ macros you must include a custom implementation before including the file if set to 0.

I have tested on Linux using GCC with the following flags
```
-ldl -lpthread -pedantic -std=c99 -Wall -Werror -Wextra -Wunused -Wno-unused-function
```
## Usage Example
This is a simple hello world example using the plugin API you can use the provided Makefile to compile it with live reload enabled.

***main.c***
```c
#define HTC_ALLOC_IMPLEMENTATION 1
#define HTC_FILE_IMPLEMENTATION 1
#define HTC_LOGGER_IMPLEMENTATION 1
#define HTC_PLUGIN_IMPLEMENTATION 1
#define HTC_PLUGIN_STATIC 1 /* Change to 1 for static compiling */

#include "htc_alloc.h"
#include "htc_file.h"
#include "htc_logger.h"
#include "htc_plugin.h"

typedef struct plugin_state_t {
	htc_allocator_i *allocator;
	htc_file_system_i *file_system;
	htc_logger_i *logger;
} plugin_state_t;

int main(int argc, char *argv[]) {
	htc_allocator_i allocator;
	htc_file_system_i fs;
	htc_logger_i logger;
	htc_plugin_t plugin;
	char *plugin_path = NULL;
	plugin_state_t state;
	htc_file_stat_t current_stat, new_stat;

	if (!htc_allocator_init(&allocator)) {
		return 1;
	}

	if (!htc_file_system_init(&fs)) {
		return 1;
	}

	if (!htc_logger_init(&logger)) {
		return 1;
	}

	state.allocator = &allocator;
	state.file_system = &fs;
	state.logger = &logger;

	plugin.state = &state;

	if (HTC_PLUGIN_STATIC != 0) {
		plugin.func = &htc_plugin_main;
	}

	if (argc == 2) {
		plugin_path = argv[1];
	} else {
		plugin_path = (char *)"./bin/libmain.so";
	}

	current_stat = fs.stat(&fs, plugin_path);

	if (current_stat.exists == false && HTC_PLUGIN_STATIC == 0) {
		logger.printf(&logger, "Plugin does not exist");

		return 1;
	}

	if (!htc_plugin_open(&plugin, plugin_path)) {
		logger.printf(&logger, "Failed to load main plugin: %s", plugin_path);

		return 1;
	}

	while (plugin.finished == false) {
		new_stat = fs.stat(&fs, plugin_path);

		if (fs.file_time_delta(&fs, current_stat.modified_time, new_stat.modified_time) == 0) {
			if (!htc_plugin_update(&plugin, false)) {
				logger.printf(&logger, "Failed during plugin update");
			}
		} else {
			logger.printf(&logger, "Reloading: %s", plugin_path);

			current_stat = new_stat;

			if (!htc_plugin_update(&plugin, true)) {
				logger.printf(&logger, "Failed during plugin update (reload)");
			}
		}
	}

	htc_plugin_close(&plugin);

	return 0;
}
```
***plugin.c***
```c
#include "htc_alloc.h"
#include "htc_file.h"
#include "htc_logger.h"
#include "htc_plugin.h"

typedef struct plugin_state_t {
	htc_allocator_i *allocator;
	htc_file_system_i *file_system;
	htc_logger_i *logger;
} plugin_state_t;

bool plugin_load(plugin_state_t *state) {
	state->logger->printf(state->logger, "Hello World");

	return true;
}

void htc_plugin_main(htc_plugin_t *plugin, enum htc_plugin_operation operation) {
	switch (operation) {
	case HTC_PLUGIN_OPERATION_LOAD:
		plugin->finished = plugin_load(plugin->state);
		break;
	default:
		plugin->finished = true;
		break;
	}
}
```
## License
```
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
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND Infringement.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
```
