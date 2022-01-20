#define HTC_ALLOC_IMPLEMENTATION 1
#define HTC_FILE_IMPLEMENTATION 1
#define HTC_LOGGER_IMPLEMENTATION 1
#define HTC_PLUGIN_IMPLEMENTATION 1
#define HTC_PLUGIN_STATIC 0 /* Change to 1 for static compiling */

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
