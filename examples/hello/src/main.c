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
