/*
# htc_time.h - v0.7.0
Licensing information can be found at the end of this file.

A simple header-only date and time library.
*/

#if !defined(HTC_TIME_H)
#define HTC_TIME_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_TIME_EXPORT)
#define HTC_TIME_EXPORT extern /* API inline, static, or extern */
#endif

#if !defined(HTC_TIME_HAVE_STDBOOL)
#define HTC_TIME_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_TIME_HAVE_STDDEF)
#define HTC_TIME_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_TIME_HAVE_STDINT)
#define HTC_TIME_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_TIME_USE_DOUBLE) || HTC_TIME_USE_DOUBLE != 0
typedef float htc_time_float_t; /* Use float by default */
#else
typedef double htc_time_float_t; /* Use double if HTC_TIME_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_TIME_LANGUAGE_C)
#define HTC_TIME_LANGUAGE_C 0
#endif

#if !defined(HTC_TIME_LANGUAGE_CPP)
#define HTC_TIME_LANGUAGE_CPP 0
#endif

#if !defined(HTC_TIME_PLATFORM_ANDROID)
#define HTC_TIME_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_TIME_PLATFORM_LINUX)
#define HTC_TIME_PLATFORM_LINUX 0
#endif

#if !defined(HTC_TIME_PLATFORM_WINDOWS)
#define HTC_TIME_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_TIME_PLATFORM_UNSUPPORTED)
#define HTC_TIME_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_TIME_LANGUAGE_C
#define HTC_TIME_LANGUAGE_C 1
#else
#undef HTC_TIME_LANGUAGE_CPP
#define HTC_TIME_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_TIME_PLATFORM_ANDROID
#define HTC_TIME_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_TIME_PLATFORM_LINUX
#define HTC_TIME_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_TIME_PLATFORM_WINDOWS
#define HTC_TIME_PLATFORM_WINDOWS 1
#else
#undef HTC_TIME_PLATFORM_UNSUPPORTED
#define HTC_TIME_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_TIME_PLATFORM_ANDROID || HTC_TIME_PLATFORM_LINUX
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
#if HTC_TIME_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_TIME_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_TIME_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_TIME_UNUSED)
#define HTC_TIME_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* Time API */
typedef struct htc_time_o {
	uint32_t opaque[4];
} htc_time_o;

/* Represents time in UTC */
typedef struct htc_date_t {
	uint8_t day;      /* Day of the month */
	uint8_t hour;     /* Hours since 12am */
	uint8_t minute;   /* Minutes */
	uint8_t month;    /* Month */
	uint8_t second;   /* Seconds */
	uint8_t week_day; /* Day of the week starting at Sunday */
	uint32_t year;    /* Year */
} htc_date_t;

HTC_TIME_EXPORT htc_date_t htc_time_date(void);

HTC_TIME_EXPORT htc_time_float_t htc_time_delta(htc_time_o from, htc_time_o to);

HTC_TIME_EXPORT htc_time_o htc_time_now(void);

HTC_TIME_EXPORT bool htc_time_sleep(htc_time_float_t seconds);

/* Seconds elapsed since January 1, 1970 (midnight UTC) not counting leap seconds.
This may be slightly off but should work as timestamps in most use cases. */
HTC_TIME_EXPORT uint64_t htc_time_timestamp(void);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_TIME_IMPLEMENTATION)
#undef HTC_TIME_IMPLEMENTATION

/* Linux */
#if HTC_TIME_PLATFORM_ANDROID == 1 || HTC_TIME_PLATFORM_LINUX == 1
#include <time.h> /* Time functions */
#elif HTC_TIME_PLATFORM_WINDOWS == 1
#define WIN32_LEAN_AND_MEAN 1

#include <time.h>    /* Time functions */
#include <windows.h> /* Windows stuff */
#endif

/* Time API */
htc_date_t htc_time_date(void) {
	htc_date_t date;

#if HTC_TIME_PLATFORM_ANDROID == 1 || HTC_TIME_PLATFORM_LINUX == 1 || HTC_TIME_PLATFORM_WINDOWS == 1
	time_t current_time = time(NULL);
	struct tm *tm = gmtime(&current_time);

	date.day = tm->tm_mday;
	date.hour = tm->tm_hour;
	date.minute = tm->tm_min;
	date.month = tm->tm_mon + 1;
	date.second = tm->tm_sec;
	date.week_day = tm->tm_wday + 1;
	date.year = tm->tm_year + 1900;
#endif

	return date;
}

htc_time_float_t htc_time_delta(htc_time_o from, htc_time_o to) {
#if HTC_TIME_PLATFORM_ANDROID == 1 || HTC_TIME_PLATFORM_LINUX == 1
	uint32_t seconds = to.opaque[0] - from.opaque[0];
	uint32_t milliseconds = to.opaque[1] - from.opaque[1];

	return (htc_time_float_t)(((seconds * 1000) + milliseconds) / (double)1000);
#elif HTC_TIME_PLATFORM_WINDOWS == 1
	LARGE_INTEGER from_milliseconds;
	LARGE_INTEGER to_milliseconds;

	from_milliseconds.QuadPart = 0;
	to_milliseconds.QuadPart = 0;

	memcpy(&(from_milliseconds.QuadPart), &(from.opaque), sizeof(LARGE_INTEGER));
	memcpy(&(to_milliseconds.QuadPart), &(to.opaque), sizeof(LARGE_INTEGER));

	return (htc_time_float_t)(to_milliseconds.QuadPart - from_milliseconds.QuadPart) / (htc_time_float_t)1000;
#endif
}

htc_time_o htc_time_now(void) {
	htc_time_o time;

#if HTC_TIME_PLATFORM_ANDROID == 1 || HTC_TIME_PLATFORM_LINUX == 1
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	time.opaque[0] = ts.tv_sec;
	time.opaque[1] = (uint32_t)(ts.tv_nsec / 1000000);
#elif HTC_TIME_PLATFORM_WINDOWS == 1
	LARGE_INTEGER counter;
	LARGE_INTEGER freq;
	LARGE_INTEGER tmp;

	QueryPerformanceCounter(&counter);
	QueryPerformanceFrequency(&freq);

	tmp.QuadPart = (counter.QuadPart * 1000) / freq.QuadPart;

	memcpy(&(time.opaque), &tmp, sizeof(LARGE_INTEGER));
#endif

	return time;
}

bool htc_time_sleep(htc_time_float_t seconds) {
	uint32_t milliseconds = (uint32_t)(seconds * 1000);

#if HTC_TIME_PLATFORM_ANDROID == 1 || HTC_TIME_PLATFORM_LINUX == 1
	long mul = 1000000;
	struct timespec rem;
	struct timespec req;

	if (seconds <= (htc_time_float_t)0.0) {
		return false;
	}

	if (milliseconds < 1000) {
		req.tv_sec = 0;

		req.tv_nsec = milliseconds * mul;
	} else {
		req.tv_sec = (int32_t)(milliseconds / 1000);

		req.tv_nsec = (milliseconds - ((uint32_t)req.tv_sec * 1000)) * mul;
	}

	if (nanosleep(&req, &rem) != 0) {
		return false;
	}

	return true;
#elif HTC_TIME_PLATFORM_WINDOWS == 1
	Sleep(milliseconds);
#endif

	return true;
}

uint64_t htc_time_timestamp(void) {
#if HTC_TIME_PLATFORM_ANDROID == 1 || HTC_TIME_PLATFORM_LINUX == 1 || HTC_TIME_PLATFORM_WINDOWS == 1
	return (uint64_t)time(NULL);
#endif

	return 0;
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
