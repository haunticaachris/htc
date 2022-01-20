/*
# htc_file.h - v0.5.0
Licensing information can be found at the end of this file.

A simple header-only file library with file system support.
*/

#if !defined(HTC_FILE_H)
#define HTC_FILE_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_FILE_EXPORT)
#define HTC_FILE_EXPORT extern /* API inline, static, or extern */
#endif

#if !defined(HTC_FILE_HAVE_STDBOOL)
#define HTC_FILE_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_FILE_HAVE_STDDEF)
#define HTC_FILE_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_FILE_HAVE_STDINT)
#define HTC_FILE_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_FILE_PATH_STRLEN)
#define HTC_FILE_PATH_STRLEN 256 /* The max string length of a file path */
#endif

#if !defined(HTC_FILE_USE_DOUBLE) || HTC_FILE_USE_DOUBLE != 0
typedef float htc_file_float_t; /* Use float by default */
#else
typedef double htc_file_float_t; /* Use double if HTC_FILE_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_FILE_LANGUAGE_C)
#define HTC_FILE_LANGUAGE_C 0
#endif

#if !defined(HTC_FILE_LANGUAGE_CPP)
#define HTC_FILE_LANGUAGE_CPP 0
#endif

#if !defined(HTC_FILE_PLATFORM_ANDROID)
#define HTC_FILE_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_FILE_PLATFORM_LINUX)
#define HTC_FILE_PLATFORM_LINUX 0
#endif

#if !defined(HTC_FILE_PLATFORM_WINDOWS)
#define HTC_FILE_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_FILE_PLATFORM_UNSUPPORTED)
#define HTC_FILE_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_FILE_LANGUAGE_C
#define HTC_FILE_LANGUAGE_C 1
#else
#undef HTC_FILE_LANGUAGE_CPP
#define HTC_FILE_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_FILE_PLATFORM_ANDROID
#define HTC_FILE_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_FILE_PLATFORM_LINUX
#define HTC_FILE_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_FILE_PLATFORM_WINDOWS
#define HTC_FILE_PLATFORM_WINDOWS 1
#else
#undef HTC_FILE_PLATFORM_UNSUPPORTED
#define HTC_FILE_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_FILE_PLATFORM_ANDROID || HTC_FILE_PLATFORM_LINUX
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
#if HTC_FILE_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_FILE_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_FILE_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_FILE_UNUSED)
#define HTC_FILE_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* File API */
typedef struct htc_file_allocator_o {
	uint32_t opaque[8];
} htc_file_allocator_o;

typedef struct htc_file_allocator_i {
	htc_file_allocator_o o;
	void *(*realloc)(struct htc_file_allocator_i *allocator, void *ptr, size_t old_size, size_t new_size);
} htc_file_allocator_i;

typedef struct htc_file_o {
	uint32_t opaque[4];
} htc_file_o;

typedef struct htc_file_i htc_file_i;

struct htc_file_i {
	htc_file_o o;
	bool (*close)(htc_file_i *f);
	bool (*is_valid)(htc_file_i *f);
	/* Returns bytes successfully read */
	size_t (*read)(htc_file_i *f, void *buffer, size_t bytes);
	bool (*set_position)(htc_file_i *f, size_t position);
	size_t (*size)(htc_file_i *f);
	bool (*write)(htc_file_i *f, const void *buffer, size_t bytes);
};

HTC_FILE_EXPORT bool htc_file_close(htc_file_i *f);

HTC_FILE_EXPORT bool htc_file_is_valid(htc_file_i *f);

HTC_FILE_EXPORT size_t htc_file_read(htc_file_i *f, void *buffer, size_t bytes);

HTC_FILE_EXPORT bool htc_file_set_position(htc_file_i *f, size_t position);

HTC_FILE_EXPORT size_t htc_file_size(htc_file_i *f);

HTC_FILE_EXPORT bool htc_file_write(htc_file_i *f, const void *buffer, size_t bytes);

/* File System API */
enum htc_file_type { HTC_FILE_TYPE_DIRECTORY, HTC_FILE_TYPE_REGULAR, HTC_FILE_TYPE_SYMLINK, HTC_FILE_TYPE_OTHER };

typedef struct htc_file_system_o {
	uint32_t opaque[8];
} htc_file_system_o;

typedef struct htc_file_system_time_o {
	uint32_t opaque[4];
} htc_file_system_time_o;

typedef struct htc_file_stat_t htc_file_stat_t;
typedef struct htc_file_system_i htc_file_system_i;

struct htc_file_system_i {
	htc_file_system_o inst;
	char *(*app_directory)(htc_file_system_i *f, htc_file_allocator_i *allocator);
	bool (*change_directory)(htc_file_system_i *f, const char *path);
	size_t (*directory_entries)(htc_file_system_i *f, const char *path, char **entries, htc_file_allocator_i *allocator);
	char *(*get_directory)(htc_file_system_i *f, htc_file_allocator_i *allocator);
	bool (*make_directory)(htc_file_system_i *f, const char *path);
	htc_file_i (*open_input)(htc_file_system_i *fs, const char *path);
	htc_file_i (*open_output)(htc_file_system_i *fs, const char *path, bool append);
	bool (*remove_directory)(htc_file_system_i *f, const char *path);
	bool (*remove_file)(htc_file_system_i *f, const char *path);
	bool (*rename)(htc_file_system_i *f, const char *old_name, const char *new_name);
	htc_file_stat_t (*stat)(htc_file_system_i *f, const char *path);
	htc_file_float_t (*file_time_delta)(htc_file_system_i *f, htc_file_system_time_o from, htc_file_system_time_o to);
};

struct htc_file_stat_t {
	bool exists;
	htc_file_system_time_o modified_time;
	size_t size;
	enum htc_file_type type;
};

HTC_FILE_EXPORT char *htc_file_system_app_directory(htc_file_system_i *f, htc_file_allocator_i *allocator);

HTC_FILE_EXPORT bool htc_file_system_change_directory(htc_file_system_i *f, const char *path);

HTC_FILE_EXPORT size_t htc_file_system_directory_entries(htc_file_system_i *f, const char *path, char **entries,
                                                         htc_file_allocator_i *allocator);

HTC_FILE_EXPORT char *htc_file_system_get_directory(htc_file_system_i *f, htc_file_allocator_i *allocator);

HTC_FILE_EXPORT bool htc_file_system_init(htc_file_system_i *f);

HTC_FILE_EXPORT bool htc_file_system_make_directory(htc_file_system_i *f, const char *path);

HTC_FILE_EXPORT htc_file_i htc_file_system_open_input(htc_file_system_i *fs, const char *path);

HTC_FILE_EXPORT htc_file_i htc_file_system_open_output(htc_file_system_i *fs, const char *path, bool append);

HTC_FILE_EXPORT bool htc_file_system_remove_directory(htc_file_system_i *f, const char *path);

HTC_FILE_EXPORT bool htc_file_system_remove_file(htc_file_system_i *f, const char *path);

HTC_FILE_EXPORT bool htc_file_system_rename(htc_file_system_i *f, const char *old_name, const char *new_name);

HTC_FILE_EXPORT htc_file_stat_t htc_file_system_stat(htc_file_system_i *f, const char *path);

HTC_FILE_EXPORT htc_file_float_t htc_file_system_time_delta(htc_file_system_i *f, htc_file_system_time_o from,
                                                            htc_file_system_time_o to);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_FILE_IMPLEMENTATION)
#undef HTC_FILE_IMPLEMENTATION

#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
#include <dirent.h>   /* Directory entries */
#include <stdio.h>    /* For io functions */
#include <stdlib.h>   /* malloc / free functions */
#include <string.h>   /* For memcpy */
#include <sys/stat.h> /* For mkdir function */
#include <time.h>     /* Linux time functions */
#include <unistd.h>   /* For rmdir / access / read / STDIN_FILENO */
#elif HTC_FILE_PLATFORM_WINDOWS == 1
#pragma comment(lib, "Ole32.lib")   /* OLE allocator lib for app_directory */
#pragma comment(lib, "Shell32.lib") /* Shell lib for app_directory */

#define WIN32_LEAN_AND_MEAN 1

#include <initguid.h>     /* Redefine the DEFINE_GUID macro */

/* initguid.h must go before KnownFolders.h */
#include <KnownFolders.h> /* Folder paths */
#include <shlobj.h>       /* Shell library for app_directory */
#include <stdio.h>        /* For io functions */
#include <sys/stat.h>     /* Stat64 function */
#include <sys/types.h>    /* Stat struct */
#include <time.h>         /* Stat time */
#include <windows.h>      /* Windows stuff */

/* REFKNOWNFOLDERID is a reference in c++ and pointer in c */
#if !defined(HTC_WINDOWS_APP_DIRECTORY)
#if HTC_LANGUAGE_C == 1
#define HTC_WINDOWS_APP_DIRECTORY &FOLDERID_LocalAppData
#else
#define HTC_WINDOWS_APP_DIRECTORY FOLDERID_LocalAppData
#endif
#endif
#endif

/* File API */
bool htc_file_close(htc_file_i *f) {
	FILE *file = NULL;

	if (f == NULL) {
		return false;
	}

	memcpy(&file, &(f->o.opaque), sizeof(FILE *));

	if (file != NULL) {
		if (fclose(file) == EOF) {
			return false;
		}

		memset(&(f->o), 0, sizeof(htc_file_o));
	}

	return true;
}

bool htc_file_is_valid(htc_file_i *f) {
	FILE *file = NULL;

	if (f == NULL) {
		return false;
	}

	memcpy(&file, &(f->o.opaque), sizeof(FILE *));

	if (file != NULL) {
		return true;
	}

	return false;
}

size_t htc_file_read(htc_file_i *f, void *buffer, size_t bytes) {
	uint8_t *buf = (uint8_t *)buffer;
	FILE *file = NULL;
	size_t read = 0;
	size_t ret = 0;

	if (f == NULL || buffer == NULL || bytes == 0) {
		return 0;
	}

	memcpy(&file, &(f->o.opaque), sizeof(FILE *));

	if (file == NULL) {
		return 0;
	}

	do {
		read = fread(&(buf[ret]), 1, bytes - ret, file);
		ret += read;
	} while (read > 0 && ret != bytes);

	if (ret != bytes) {
		return 0;
	}

	return ret;
}

bool htc_file_set_position(htc_file_i *f, size_t position) {
	FILE *file = NULL;

	if (!htc_file_is_valid(f)) {
		return false;
	}

	memcpy(&file, &(f->o.opaque), sizeof(FILE *));

	if (fseek(file, position, SEEK_SET) != 0) {
		return false;
	}

	return true;
}

size_t htc_file_size(htc_file_i *f) {
	size_t current_position = 0;
	FILE *file = NULL;
	size_t size = 0;

	if (f == NULL) {
		return size;
	}

	memcpy(&file, &(f->o.opaque), sizeof(FILE *));

	if (file == NULL) {
		return size;
	}

	current_position = ftell(file);

	fseek(file, 0, SEEK_END);

	size = ftell(file);

	fseek(file, current_position, SEEK_SET);

	return size;
}

bool htc_file_write(htc_file_i *f, const void *buffer, size_t bytes) {
	FILE *file = NULL;
	size_t written = 0;

	if (f == NULL || buffer == NULL || bytes == 0) {
		return false;
	}

	memcpy(&file, &(f->o.opaque), sizeof(void *));

	if (file == NULL) {
		return false;
	}

	written = fwrite(buffer, 1, bytes, file);

	fflush(file);

	if (written != bytes) {
		return false;
	}

	return true;
}

/* File System API */
/*
Creates a new buffer so it can be changed or modified.
Buffer size is the string length.
*/
char *htc_file_system_app_directory(htc_file_system_i *f, htc_file_allocator_i *allocator) {
#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
	char *buffer;
	char *dir;

	if (f == NULL || allocator == NULL) {
		return NULL;
	}

	dir = (char *)getenv("HOME");

	if (dir == NULL) {
		return NULL;
	}

	buffer = (char *)allocator->realloc(allocator, NULL, 0, strlen(dir));

	memcpy(buffer, dir, strlen(dir));

	return buffer;
#elif HTC_FILE_PLATFORM_WINDOWS == 1
	char *buffer;
	PWSTR dir = NULL;

	if (f == NULL || allocator == NULL) {
		return NULL;
	}

	if (SHGetKnownFolderPath(HTC_WINDOWS_APP_DIRECTORY, 0, NULL, &dir) != S_OK) {
		return NULL;
	}

	buffer = (char *)allocator->realloc(allocator, NULL, 0, strlen(dir));

	if (wcstombs((char *)buffer, dir, strlen(dir)) == (size_t)-1) {
		CoTaskMemFree(dir);

		return NULL;
	}

	CoTaskMemFree(dir);

	return buffer;
#endif
}

bool htc_file_system_change_directory(htc_file_system_i *f, const char *path) {
	if (f == NULL || path == NULL) {
		return false;
	}

#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
	if (chdir(path) != -1) {
		return true;
	}
#elif HTC_FILE_PLATFORM_WINDOWS == 1
	if (SetCurrentDirectory(path) != 0) {
		return true;
	}
#endif

	return false;
}

/*
Buffer size is determined by the user.
*/
size_t htc_file_system_directory_entries(htc_file_system_i *f, const char *path, char **entries,
                                         htc_file_allocator_i *allocator) {
#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
	size_t count = 0;
	DIR *dir;
	struct dirent *entry;

	if (f == NULL || path == NULL || entries == NULL || allocator == NULL) {
		return 0;
	}

	dir = opendir(path);

	if (dir == NULL) {
		return count;
	}

	while ((entry = readdir(dir)) != NULL) {
		entries[count] = (char *)allocator->realloc(allocator, NULL, 0, strlen(entry->d_name) + 1);
		strcpy(entries[count], entry->d_name);

		count += 1;
	}

	closedir(dir);

	return count;
#elif HTC_FILE_PLATFORM_WINDOWS == 1
	size_t count = 0;
	/*
	TODO: port api with allocator used and
	clean up code
	
	size_t count = 0;
	WIN32_FIND_DATA data;
	HANDLE handle = INVALID_HANDLE_VALUE;
	uint8_t path_buffer[HTC_FILE_PATH_STRLEN];
	char *pc = NULL;
	uint32_t pos = 0;
	uint32_t str_size = 0;

	if (f == NULL || path == NULL || entries == NULL || a == NULL) {
		return 0;
	}

	str_size = strlen(path);

	path_buffer[str_size] = '\0';

	pc = strstr((char *)path_buffer, "\\");

	while (pc != NULL) {
		pc[0] = '/';
		pc = strstr((char *)path_buffer, "\\");
	}

	memcpy(&path_buffer, path, str_size);
	strcat((char *)(&path_buffer), "/*");

	handle = FindFirstFileA((LPCSTR)path_buffer, &data);

	if (handle == INVALID_HANDLE_VALUE) {
		return count;
	}

	str_size = strlen(data.cFileName);

	if ((pos + str_size + 1) >= size) {
		return count;
	}

	memcpy(&(buffer[pos]), data.cFileName, str_size);

	buffer[pos + str_size] = '\0';

	pos += str_size + 1;

	count += 1;

	while (FindNextFileA(handle, &data) != 0) {
		str_size = strlen(data.cFileName);

		if ((pos + str_size + 1) >= size) {
			return count;
		}

		memcpy(&(buffer[pos]), data.cFileName, str_size);

		buffer[pos + str_size] = '\0';

		pos += str_size + 1;

		count += 1;
	}

	FindClose(handle);
*/
	return count;
#endif
}

char *htc_file_system_get_directory(htc_file_system_i *f, htc_file_allocator_i *allocator) {
	char *buffer = NULL;
	char *tmp = NULL;

	if (f == NULL || allocator == NULL) {
		return NULL;
	}

	tmp = (char *)allocator->realloc(allocator, NULL, 0, HTC_FILE_PATH_STRLEN);

#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
	getcwd((char *)tmp, HTC_FILE_PATH_STRLEN);
#elif HTC_FILE_PLATFORM_WINDOWS == 1
	if (GetCurrentDirectory(HTC_FILE_PATH_STRLEN, (char *)tmp) == 0) {
		return NULL;
	}
#endif

	buffer = (char *)allocator->realloc(allocator, NULL, 0, strlen(tmp) + 1);

	strcpy(buffer, tmp);

	allocator->realloc(allocator, tmp, HTC_FILE_PATH_STRLEN, 0);

	return buffer;
}

bool htc_file_system_init(htc_file_system_i *f) {
	if (f == NULL) {
		return false;
	}

	f->app_directory = &htc_file_system_app_directory;
	f->change_directory = &htc_file_system_change_directory;
	f->directory_entries = &htc_file_system_directory_entries;
	f->get_directory = &htc_file_system_get_directory;
	f->make_directory = &htc_file_system_make_directory;
	f->open_input = &htc_file_system_open_input;
	f->open_output = &htc_file_system_open_output;
	f->remove_directory = &htc_file_system_remove_directory;
	f->remove_file = &htc_file_system_remove_file;
	f->rename = &htc_file_system_rename;
	f->stat = &htc_file_system_stat;
	f->file_time_delta = &htc_file_system_time_delta;

	return true;
}

bool htc_file_system_make_directory(htc_file_system_i *f, const char *path) {
	if (f == NULL || path == NULL) {
		return false;
	}

#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
	if (mkdir(path, 0700) == 0) {
		return true;
	}
#elif HTC_FILE_PLATFORM_WINDOWS == 1
	if (CreateDirectoryA(path, NULL) != 0) {
		return true;
	}
#endif

	return false;
}

htc_file_i htc_file_system_open_input(htc_file_system_i *fs, const char *path) {
	htc_file_i f;
	FILE *file = NULL;

	memset(&f, 0, sizeof(htc_file_i));

	if (fs == NULL || path == NULL) {
		return f;
	}

	f.close = &htc_file_close;
	f.is_valid = &htc_file_is_valid;
	f.read = &htc_file_read;
	f.set_position = &htc_file_set_position;
	f.size = &htc_file_size;
	f.write = &htc_file_write;

	if (path == NULL) {
		return f;
	}

	file = fopen(path, "r+");

	if (file == NULL) {
		return f;
	}

	memcpy(&(f.o.opaque), &file, sizeof(FILE *));

	return f;
}

htc_file_i htc_file_system_open_output(htc_file_system_i *fs, const char *path, bool append) {
	htc_file_i f;
	FILE *file = NULL;

	memset(&f, 0, sizeof(htc_file_i));

	if (fs == NULL) {
		return f;
	}

	f.close = &htc_file_close;
	f.is_valid = &htc_file_is_valid;
	f.read = &htc_file_read;
	f.set_position = &htc_file_set_position;
	f.size = &htc_file_size;
	f.write = &htc_file_write;

	if (path == NULL) {
		return f;
	}

	if (!append) {
		file = fopen(path, "w+");
	} else {
		file = fopen(path, "a+");
	}

	if (file == NULL) {
		return f;
	}

	memcpy(&(f.o.opaque), &file, sizeof(FILE *));

	return f;
}

bool htc_file_system_remove_directory(htc_file_system_i *f, const char *path) {
	if (f == NULL || path == NULL) {
		return false;
	}

#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
	if (rmdir(path) == 0) {
		return true;
	}
#elif HTC_FILE_PLATFORM_WINDOWS == 1
	if (RemoveDirectoryA(path) != 0) {
		return true;
	}
#endif

	return false;
}

bool htc_file_system_remove_file(htc_file_system_i *f, const char *path) {
	if (f == NULL || path == NULL) {
		return false;
	}

#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
	if (remove(path) == 0) {
		return true;
	}
#elif HTC_FILE_PLATFORM_WINDOWS == 1
	if (remove(path) == 0) {
		return true;
	}
#endif

	return false;
}

bool htc_file_system_rename(htc_file_system_i *f, const char *old_name, const char *new_name) {
	if (f == NULL || old_name == NULL || new_name == NULL) {
		return false;
	}

#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1 || HTC_FILE_PLATFORM_WINDOWS == 1
	if (rename(old_name, new_name) == 0) {
		return true;
	}
#endif

	return false;
}

htc_file_stat_t htc_file_system_stat(htc_file_system_i *f, const char *path) {
#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1
	struct stat buf;
	htc_file_stat_t st;

	st.exists = false;
	st.modified_time.opaque[0] = 0;
	st.modified_time.opaque[1] = 0;
	st.size = 0;
	st.type = HTC_FILE_TYPE_OTHER;

	if (f == NULL || path == NULL) {
		return st;
	}

	if (stat(path, &buf) != 0) {
		return st;
	}

	st.exists = true;

	memcpy(&(st.modified_time.opaque), &(buf.st_mtime), sizeof(time_t));

	st.size = buf.st_size;

	if (S_ISDIR(buf.st_mode)) {
		st.type = HTC_FILE_TYPE_DIRECTORY;
	}

	if (S_ISREG(buf.st_mode)) {
		st.type = HTC_FILE_TYPE_REGULAR;
	}

	if (S_ISLNK(buf.st_mode)) {
		st.type = HTC_FILE_TYPE_SYMLINK;
	}
#elif HTC_FILE_PLATFORM_WINDOWS == 1
	struct __stat64 buf;
	htc_file_stat_t st;

	st.exists = false;
	st.modified_time.opaque[0] = 0;
	st.modified_time.opaque[1] = 0;
	st.size = 0;
	st.type = HTC_FILE_TYPE_OTHER;

	if (f == NULL || path == NULL) {
		return st;
	}

	if (_stat64(path, &buf) != 0) {
		return st;
	}

	st.exists = true;

	memcpy(&(st.modified_time.opaque), &(buf.st_mtime), sizeof(time_t));

	st.size = buf.st_size;

	if (_S_IFDIR & buf.st_mode) {
		st.type = HTC_FILE_TYPE_DIRECTORY;
	}

	if (_S_IFREG & buf.st_mode) {
		st.type = HTC_FILE_TYPE_REGULAR;
	}
#endif

	return st;
}

htc_file_float_t htc_file_system_time_delta(htc_file_system_i *f, htc_file_system_time_o from,
                                            htc_file_system_time_o to) {
#if HTC_FILE_PLATFORM_ANDROID == 1 || HTC_FILE_PLATFORM_LINUX == 1 || HTC_FILE_PLATFORM_WINDOWS == 1
	time_t start;
	time_t end;

	if (f == NULL) {
		return 0;
	}

	memcpy(&start, &(from.opaque), sizeof(time_t));
	memcpy(&end, &(to.opaque), sizeof(time_t));

	return (htc_file_float_t)difftime(end, start);
#endif

	return 0.0;
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
