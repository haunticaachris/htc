/*
# htc_math.h - v0.6.0
Licensing information can be found at the end of this file.

A simple header-only math library for games. This library
is not optimized at all and is meant mainly for testing code.
*/

#if !defined(HTC_MATH_H)
#define HTC_MATH_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_MATH_EXPORT)
#define HTC_MATH_EXPORT inline /* Inline by default*/
#endif

#if !defined(HTC_MATH_HAVE_STDBOOL)
#define HTC_MATH_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_MATH_HAVE_STDDEF)
#define HTC_MATH_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_MATH_HAVE_STDINT)
#define HTC_MATH_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_MATH_INFINITY)
#define HTC_MATH_INFINITY 1e8 /* Infinity */
#endif

#if !defined(HTC_MATH_PI)
#define HTC_MATH_PI 3.141592653589 /* Pi */
#endif

#if !defined(HTC_MATH_USE_DOUBLE) || HTC_MATH_USE_DOUBLE != 0
typedef float htc_math_float_t; /* Use float by default */
#else
typedef double htc_math_float_t; /* Use double if HTC_MATH_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_MATH_LANGUAGE_C)
#define HTC_MATH_LANGUAGE_C 0
#endif

#if !defined(HTC_MATH_LANGUAGE_CPP)
#define HTC_MATH_LANGUAGE_CPP 0
#endif

#if !defined(HTC_MATH_PLATFORM_ANDROID)
#define HTC_MATH_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_MATH_PLATFORM_LINUX)
#define HTC_MATH_PLATFORM_LINUX 0
#endif

#if !defined(HTC_MATH_PLATFORM_WINDOWS)
#define HTC_MATH_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_MATH_PLATFORM_UNSUPPORTED)
#define HTC_MATH_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_MATH_LANGUAGE_C
#define HTC_MATH_LANGUAGE_C 1
#else
#undef HTC_MATH_LANGUAGE_CPP
#define HTC_MATH_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_MATH_PLATFORM_ANDROID
#define HTC_MATH_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_MATH_PLATFORM_LINUX
#define HTC_MATH_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_MATH_PLATFORM_WINDOWS
#define HTC_MATH_PLATFORM_WINDOWS 1
#else
#undef HTC_MATH_PLATFORM_UNSUPPORTED
#define HTC_MATH_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_MATH_PLATFORM_ANDROID || HTC_MATH_PLATFORM_LINUX
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
#if HTC_MATH_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_MATH_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_MATH_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_MATH_DEG_TO_RAD)
#define HTC_MATH_DEG_TO_RAD (HTC_MATH_PI / 180) /* Degrees to radians */
#endif

#if !defined(HTC_MATH_RAD_TO_DEG)
#define HTC_MATH_RAD_TO_DEG (180 / HTC_MATH_PI) /* Radians to degrees */
#endif

#if !defined(HTC_MATH_UNUSED)
#define HTC_MATH_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* Math API */
typedef struct htc_math_mat44_t {
	htc_math_float_t m[16];
} htc_math_mat44_t;

typedef struct htc_math_vec2_t {
	htc_math_float_t x, y;
} htc_math_vec2_t;

typedef struct htc_math_vec3_t {
	htc_math_float_t x, y, z;
} htc_math_vec3_t;

typedef struct htc_math_vec4_t {
	htc_math_float_t x, y, z, w;
} htc_math_vec4_t;

HTC_MATH_EXPORT htc_math_float_t htc_math_clampf(htc_math_float_t x, htc_math_float_t min, htc_math_float_t max);

HTC_MATH_EXPORT htc_math_float_t htc_math_lerp(htc_math_float_t start, htc_math_float_t end, htc_math_float_t t);

HTC_MATH_EXPORT htc_math_mat44_t htc_math_mat44_mul(htc_math_mat44_t m, htc_math_mat44_t mul);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_add(htc_math_vec2_t v, htc_math_vec2_t add);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_adds(htc_math_vec2_t v, htc_math_float_t add);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_div(htc_math_vec2_t v, htc_math_vec2_t div);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_divs(htc_math_vec2_t v, htc_math_float_t div);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec2_dot(htc_math_vec2_t v1, htc_math_vec2_t v2);

HTC_MATH_EXPORT bool htc_math_vec2_equal(htc_math_vec2_t v1, htc_math_vec2_t v2);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec2_len(htc_math_vec2_t v);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec2_len_sqr(htc_math_vec2_t v);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_lerp(htc_math_vec2_t start, htc_math_vec2_t end, htc_math_float_t t);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_mul(htc_math_vec2_t v, htc_math_vec2_t mul);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_muls(htc_math_vec2_t v, htc_math_float_t mul);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_norm(htc_math_vec2_t v);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_set(htc_math_float_t x, htc_math_float_t y);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_sub(htc_math_vec2_t v, htc_math_vec2_t sub);

HTC_MATH_EXPORT htc_math_vec2_t htc_math_vec2_subs(htc_math_vec2_t v, htc_math_float_t sub);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_add(htc_math_vec3_t v, htc_math_vec3_t add);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_adds(htc_math_vec3_t v, htc_math_float_t add);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_cross(htc_math_vec3_t v1, htc_math_vec3_t v2);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_div(htc_math_vec3_t v, htc_math_vec3_t div);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_divs(htc_math_vec3_t v, htc_math_float_t div);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec3_dot(htc_math_vec3_t v1, htc_math_vec3_t v2);

HTC_MATH_EXPORT bool htc_math_vec3_equal(htc_math_vec3_t v1, htc_math_vec3_t v2);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec3_len(htc_math_vec3_t v);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec3_len_sqr(htc_math_vec3_t v);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_lerp(htc_math_vec3_t start, htc_math_vec3_t end, htc_math_float_t t);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_mul(htc_math_vec3_t v, htc_math_vec3_t mul);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_muls(htc_math_vec3_t v, htc_math_float_t mul);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_norm(htc_math_vec3_t v);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_set(htc_math_float_t x, htc_math_float_t y, htc_math_float_t z);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_sub(htc_math_vec3_t v, htc_math_vec3_t sub);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_vec3_subs(htc_math_vec3_t v, htc_math_float_t sub);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_add(htc_math_vec4_t v, htc_math_vec4_t add);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_adds(htc_math_vec4_t v, htc_math_float_t add);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_div(htc_math_vec4_t v, htc_math_vec4_t div);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_divs(htc_math_vec4_t v, htc_math_float_t div);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec4_dot(htc_math_vec4_t v1, htc_math_vec4_t v2);

HTC_MATH_EXPORT bool htc_math_vec4_equal(htc_math_vec4_t v1, htc_math_vec4_t v2);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec4_len(htc_math_vec4_t v);

HTC_MATH_EXPORT htc_math_float_t htc_math_vec4_len_sqr(htc_math_vec4_t v);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_lerp(htc_math_vec4_t start, htc_math_vec4_t end, htc_math_float_t t);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_mul(htc_math_vec4_t v, htc_math_vec4_t mul);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_muls(htc_math_vec4_t v, htc_math_float_t mul);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_norm(htc_math_vec4_t v);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_set(htc_math_float_t x, htc_math_float_t y, htc_math_float_t z,
                                                  htc_math_float_t w);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_sub(htc_math_vec4_t v, htc_math_vec4_t sub);

HTC_MATH_EXPORT htc_math_vec4_t htc_math_vec4_subs(htc_math_vec4_t v, htc_math_float_t sub);

/* Random API */
struct htc_math_random_i;

typedef struct htc_random_o {
	uint32_t opaque[32];
} htc_random_o;

typedef struct htc_random_i {
	htc_random_o inst;
	int32_t (*next)(struct htc_random_i *r);
} htc_random_i;

HTC_MATH_EXPORT bool htc_random_init(htc_random_i *r);

HTC_MATH_EXPORT int32_t htc_random_next(htc_random_i *r);

/* Collision API */
typedef struct htc_math_circle_t {
	htc_math_vec2_t center;
	htc_math_float_t radius;
} htc_math_circle_t;

typedef struct htc_math_ray_t {
	htc_math_vec3_t direction;
	htc_math_vec3_t origin;
} htc_math_ray_t;

typedef struct htc_math_sphere_t {
	htc_math_vec3_t center;
	htc_math_float_t radius;
} htc_math_sphere_t;

HTC_MATH_EXPORT htc_math_float_t htc_math_intersect_ray_sphere(htc_math_ray_t r, htc_math_sphere_t s,
                                                               htc_math_float_t min, htc_math_float_t max,
                                                               htc_math_vec3_t *point);

HTC_MATH_EXPORT htc_math_vec3_t htc_math_ray_get_point(htc_math_ray_t r, htc_math_float_t t);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_MATH_IMPLEMENTATION)
#undef HTC_MATH_IMPLEMENTATION

#include <math.h>   /* Standard math functions */
#include <stdlib.h> /* rand functions */

/* Math API */
htc_math_float_t htc_math_clampf(htc_math_float_t x, htc_math_float_t min, htc_math_float_t max) {
	if (x < min) {
		return min;
	}

	if (x > max) {
		return max;
	}

	return x;
}

htc_math_float_t htc_math_lerp(htc_math_float_t start, htc_math_float_t end, htc_math_float_t t) {
	return start + (end - start) * t;
}

htc_math_mat44_t htc_math_mat44_mul(htc_math_mat44_t m, htc_math_mat44_t mul) {
	htc_math_mat44_t mat;

	htc_math_float_t tmp0 = m.m[0];
	htc_math_float_t tmp1 = m.m[1];
	htc_math_float_t tmp2 = m.m[2];
	htc_math_float_t tmp3 = m.m[3];

	mat.m[0] = tmp0 * mul.m[0] + tmp1 * mul.m[4] + tmp2 * mul.m[8] + tmp3 * mul.m[12];
	mat.m[1] = tmp0 * mul.m[1] + tmp1 * mul.m[5] + tmp2 * mul.m[9] + tmp3 * mul.m[13];
	mat.m[2] = tmp0 * mul.m[2] + tmp1 * mul.m[6] + tmp2 * mul.m[10] + tmp3 * mul.m[14];
	mat.m[3] = tmp0 * mul.m[3] + tmp1 * mul.m[7] + tmp2 * mul.m[11] + tmp3 * mul.m[15];

	tmp0 = m.m[4];
	tmp1 = m.m[5];
	tmp2 = m.m[6];
	tmp3 = m.m[7];

	mat.m[4] = tmp0 * mul.m[0] + tmp1 * mul.m[4] + tmp2 * mul.m[8] + tmp3 * mul.m[12];
	mat.m[5] = tmp0 * mul.m[1] + tmp1 * mul.m[5] + tmp2 * mul.m[9] + tmp3 * mul.m[13];
	mat.m[6] = tmp0 * mul.m[2] + tmp1 * mul.m[6] + tmp2 * mul.m[10] + tmp3 * mul.m[14];
	mat.m[7] = tmp0 * mul.m[3] + tmp1 * mul.m[7] + tmp2 * mul.m[11] + tmp3 * mul.m[15];

	tmp0 = m.m[8];
	tmp1 = m.m[9];
	tmp2 = m.m[10];
	tmp3 = m.m[11];

	mat.m[8] = tmp0 * mul.m[0] + tmp1 * mul.m[4] + tmp2 * mul.m[8] + tmp3 * mul.m[12];
	mat.m[9] = tmp0 * mul.m[1] + tmp1 * mul.m[5] + tmp2 * mul.m[9] + tmp3 * mul.m[13];
	mat.m[10] = tmp0 * mul.m[2] + tmp1 * mul.m[6] + tmp2 * mul.m[10] + tmp3 * mul.m[14];
	mat.m[11] = tmp0 * mul.m[3] + tmp1 * mul.m[7] + tmp2 * mul.m[11] + tmp3 * mul.m[15];

	tmp0 = m.m[12];
	tmp1 = m.m[13];
	tmp2 = m.m[14];
	tmp3 = m.m[15];

	mat.m[12] = tmp0 * mul.m[0] + tmp1 * mul.m[4] + tmp2 * mul.m[8] + tmp3 * mul.m[12];
	mat.m[13] = tmp0 * mul.m[1] + tmp1 * mul.m[5] + tmp2 * mul.m[9] + tmp3 * mul.m[13];
	mat.m[14] = tmp0 * mul.m[2] + tmp1 * mul.m[6] + tmp2 * mul.m[10] + tmp3 * mul.m[14];
	mat.m[15] = tmp0 * mul.m[3] + tmp1 * mul.m[7] + tmp2 * mul.m[11] + tmp3 * mul.m[15];

	return mat;
}

htc_math_vec2_t htc_math_vec2_add(htc_math_vec2_t v, htc_math_vec2_t add) {
	v.x += add.x;
	v.y += add.y;

	return v;
}

htc_math_vec2_t htc_math_vec2_adds(htc_math_vec2_t v, htc_math_float_t add) {
	v.x += add;
	v.y += add;

	return v;
}

htc_math_vec2_t htc_math_vec2_div(htc_math_vec2_t v, htc_math_vec2_t div) {
	v.x /= div.x;
	v.y /= div.y;

	return v;
}

htc_math_vec2_t htc_math_vec2_divs(htc_math_vec2_t v, htc_math_float_t div) {
	v.x /= div;
	v.y /= div;

	return v;
}

htc_math_float_t htc_math_vec2_dot(htc_math_vec2_t v1, htc_math_vec2_t v2) { return v1.x * v2.x + v1.y * v2.y; }

bool htc_math_vec2_equal(htc_math_vec2_t v1, htc_math_vec2_t v2) {
	if (v1.x != v2.x) {
		return false;
	}

	if (v1.y != v2.y) {
		return false;
	}

	return true;
}

htc_math_float_t htc_math_vec2_len(htc_math_vec2_t v) { return (htc_math_float_t)sqrt(v.x * v.x + v.y * v.y); }

htc_math_float_t htc_math_vec2_len_sqr(htc_math_vec2_t v) { return v.x * v.x + v.y * v.y; }

htc_math_vec2_t htc_math_vec2_lerp(htc_math_vec2_t start, htc_math_vec2_t end, htc_math_float_t t) {
	start.x = start.x + (end.x - start.x) * t;
	start.y = start.y + (end.y - start.y) * t;

	return start;
}

htc_math_vec2_t htc_math_vec2_mul(htc_math_vec2_t v, htc_math_vec2_t mul) {
	v.x *= mul.x;
	v.y *= mul.y;

	return v;
}

htc_math_vec2_t htc_math_vec2_muls(htc_math_vec2_t v, htc_math_float_t mul) {
	v.x *= mul;
	v.y *= mul;

	return v;
}

htc_math_vec2_t htc_math_vec2_norm(htc_math_vec2_t v) {
	htc_math_float_t len = htc_math_vec2_len(v);

	if (len != 0) {
		v.x /= len;
		v.y /= len;
	}

	return v;
}

htc_math_vec2_t htc_math_vec2_set(htc_math_float_t x, htc_math_float_t y) {
	htc_math_vec2_t v;

	v.x = x;
	v.y = y;

	return v;
}

htc_math_vec2_t htc_math_vec2_sub(htc_math_vec2_t v, htc_math_vec2_t sub) {
	v.x -= sub.x;
	v.y -= sub.y;

	return v;
}

htc_math_vec2_t htc_math_vec2_subs(htc_math_vec2_t v, htc_math_float_t sub) {
	v.x -= sub;
	v.y -= sub;

	return v;
}

htc_math_vec3_t htc_math_vec3_add(htc_math_vec3_t v, htc_math_vec3_t add) {
	v.x += add.x;
	v.y += add.y;
	v.z += add.z;

	return v;
}

htc_math_vec3_t htc_math_vec3_adds(htc_math_vec3_t v, htc_math_float_t add) {
	v.x += add;
	v.y += add;
	v.z += add;

	return v;
}

htc_math_vec3_t htc_math_vec3_cross(htc_math_vec3_t v1, htc_math_vec3_t v2) {
	htc_math_vec3_t v;

	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;

	return v;
}

htc_math_vec3_t htc_math_vec3_div(htc_math_vec3_t v, htc_math_vec3_t div) {
	v.x /= div.x;
	v.y /= div.y;
	v.z /= div.z;

	return v;
}

htc_math_vec3_t htc_math_vec3_divs(htc_math_vec3_t v, htc_math_float_t div) {
	v.x /= div;
	v.y /= div;
	v.z /= div;

	return v;
}

htc_math_float_t htc_math_vec3_dot(htc_math_vec3_t v1, htc_math_vec3_t v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

bool htc_math_vec3_equal(htc_math_vec3_t v1, htc_math_vec3_t v2) {
	if (v1.x != v2.x) {
		return false;
	}

	if (v1.y != v2.y) {
		return false;
	}

	if (v1.z != v2.z) {
		return false;
	}

	return true;
}

htc_math_float_t htc_math_vec3_len(htc_math_vec3_t v) {
	return (htc_math_float_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

htc_math_float_t htc_math_vec3_len_sqr(htc_math_vec3_t v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

htc_math_vec3_t htc_math_vec3_lerp(htc_math_vec3_t start, htc_math_vec3_t end, htc_math_float_t t) {
	start.x = start.x + (end.x - start.x) * t;
	start.y = start.y + (end.y - start.y) * t;
	start.z = start.z + (end.z - start.z) * t;

	return start;
}

htc_math_vec3_t htc_math_vec3_mul(htc_math_vec3_t v, htc_math_vec3_t mul) {
	v.x *= mul.x;
	v.y *= mul.y;
	v.z *= mul.z;

	return v;
}

htc_math_vec3_t htc_math_vec3_muls(htc_math_vec3_t v, htc_math_float_t mul) {
	v.x *= mul;
	v.y *= mul;
	v.z *= mul;

	return v;
}

htc_math_vec3_t htc_math_vec3_norm(htc_math_vec3_t v) {
	htc_math_float_t len = htc_math_vec3_len(v);

	if (len != 0) {
		v.x /= len;
		v.y /= len;
		v.z /= len;
	}

	return v;
}

htc_math_vec3_t htc_math_vec3_set(htc_math_float_t x, htc_math_float_t y, htc_math_float_t z) {
	htc_math_vec3_t v;

	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

htc_math_vec3_t htc_math_vec3_sub(htc_math_vec3_t v, htc_math_vec3_t sub) {
	v.x -= sub.x;
	v.y -= sub.y;
	v.z -= sub.z;

	return v;
}

htc_math_vec3_t htc_math_vec3_subs(htc_math_vec3_t v, htc_math_float_t sub) {
	v.x -= sub;
	v.y -= sub;
	v.z -= sub;

	return v;
}

htc_math_vec4_t htc_math_vec4_add(htc_math_vec4_t v, htc_math_vec4_t add) {
	v.x += add.x;
	v.y += add.y;
	v.z += add.z;
	v.w += add.w;

	return v;
}

htc_math_vec4_t htc_math_vec4_adds(htc_math_vec4_t v, htc_math_float_t add) {
	v.x += add;
	v.y += add;
	v.z += add;
	v.w += add;

	return v;
}

htc_math_vec4_t htc_math_vec4_div(htc_math_vec4_t v, htc_math_vec4_t div) {
	v.x /= div.x;
	v.y /= div.y;
	v.z /= div.z;
	v.w /= div.w;

	return v;
}

htc_math_vec4_t htc_math_vec4_divs(htc_math_vec4_t v, htc_math_float_t div) {
	v.x /= div;
	v.y /= div;
	v.z /= div;
	v.w /= div;

	return v;
}

htc_math_float_t htc_math_vec4_dot(htc_math_vec4_t v1, htc_math_vec4_t v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

bool htc_math_vec4_equal(htc_math_vec4_t v1, htc_math_vec4_t v2) {
	if (v1.x != v2.x) {
		return false;
	}

	if (v1.y != v2.y) {
		return false;
	}

	if (v1.z != v2.z) {
		return false;
	}

	if (v1.w != v2.w) {
		return false;
	}

	return true;
}

htc_math_float_t htc_math_vec4_len(htc_math_vec4_t v) {
	return (htc_math_float_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

htc_math_float_t htc_math_vec4_len_sqr(htc_math_vec4_t v) { return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w; }

htc_math_vec4_t htc_math_vec4_lerp(htc_math_vec4_t start, htc_math_vec4_t end, htc_math_float_t t) {
	start.x = start.x + (end.x - start.x) * t;
	start.y = start.y + (end.y - start.y) * t;
	start.z = start.z + (end.z - start.z) * t;
	start.w = start.w + (end.w - start.w) * t;

	return start;
}

htc_math_vec4_t htc_math_vec4_mul(htc_math_vec4_t v, htc_math_vec4_t mul) {
	v.x *= mul.x;
	v.y *= mul.y;
	v.z *= mul.z;
	v.w *= mul.w;

	return v;
}

htc_math_vec4_t htc_math_vec4_muls(htc_math_vec4_t v, htc_math_float_t mul) {
	v.x *= mul;
	v.y *= mul;
	v.z *= mul;
	v.w *= mul;

	return v;
}

htc_math_vec4_t htc_math_vec4_norm(htc_math_vec4_t v) {
	htc_math_float_t len = htc_math_vec4_len(v);

	if (len != 0) {
		v.x /= len;
		v.y /= len;
		v.z /= len;
		v.w /= len;
	}

	return v;
}

htc_math_vec4_t htc_math_vec4_set(htc_math_float_t x, htc_math_float_t y, htc_math_float_t z, htc_math_float_t w) {
	htc_math_vec4_t v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;

	return v;
}

htc_math_vec4_t htc_math_vec4_sub(htc_math_vec4_t v, htc_math_vec4_t sub) {
	v.x -= sub.x;
	v.y -= sub.y;
	v.z -= sub.z;
	v.w -= sub.w;

	return v;
}

htc_math_vec4_t htc_math_vec4_subs(htc_math_vec4_t v, htc_math_float_t sub) {
	v.x -= sub;
	v.y -= sub;
	v.z -= sub;
	v.w -= sub;

	return v;
}

/* Random API */
bool htc_random_init(htc_random_i *r) {
	if (r == NULL) {
		return false;
	}

	r->next = &htc_random_next;

	return true;
}

int32_t htc_random_next(htc_random_i *r) {
	if (r == NULL) {
		return 0;
	}

	return rand();
}

/* Collision API */
htc_math_float_t htc_math_intersect_ray_sphere(htc_math_ray_t r, htc_math_sphere_t s, htc_math_float_t min,
                                               htc_math_float_t max, htc_math_vec3_t *point) {
	htc_math_vec3_t oc = htc_math_vec3_sub(r.origin, s.center);
	htc_math_float_t a = htc_math_vec3_len_sqr(r.direction);
	htc_math_float_t b = htc_math_vec3_dot(oc, r.direction);
	htc_math_float_t c = htc_math_vec3_len_sqr(oc) - s.radius * s.radius;
	htc_math_float_t discriminant = b * b - a * c;
	htc_math_float_t sqrtd = sqrt(discriminant);
	htc_math_float_t t = (-b - sqrtd) / a;

	if (discriminant < 0) {
		return HTC_MATH_INFINITY;
	}

	if (t < min || t > max) {
		t = (-b + sqrtd) / a;

		if (t < min || t > max) {
			return HTC_MATH_INFINITY;
		}
	}

	*point = htc_math_ray_get_point(r, t);

	return t;
}

htc_math_vec3_t htc_math_ray_get_point(htc_math_ray_t r, htc_math_float_t t) {
	/* Uses origin + direction * distance */
	return htc_math_vec3_add(r.origin, htc_math_vec3_muls(r.direction, t));
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
