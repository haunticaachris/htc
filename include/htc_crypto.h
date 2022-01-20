/*
# htc_crypto.h - v0.2.0
Licensing information can be found at the end of this file.

A simple header-only crypto library (Used for testing not intended
for production use).
*/

#if !defined(HTC_CRYPTO_H)
#define HTC_CRYPTO_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* Config */
#if !defined(HTC_CRYPTO_BASE64_DECODED_SIZE)
#define HTC_CRYPTO_BASE64_DECODED_SIZE(s) ((s) - 1) * 3) / 4 + 1 /* Returns memory allocated when decoding a base64 string */
#endif

#if !defined(HTC_CRYPTO_EXPORT)
#define HTC_CRYPTO_EXPORT extern /* API inline, static, or extern */
#endif

#if !defined(HTC_CRYPTO_HAVE_STDBOOL)
#define HTC_CRYPTO_HAVE_STDBOOL 1 /* Defualt to using stdbool.h */
#endif

#if !defined(HTC_CRYPTO_HAVE_STDDEF)
#define HTC_CRYPTO_HAVE_STDDEF 1 /* Defualt to using stddef.h */
#endif

#if !defined(HTC_CRYPTO_HAVE_STDINT)
#define HTC_CRYPTO_HAVE_STDINT 1 /* Defualt to using stdint.h */
#endif

#if !defined(HTC_CRYPTO_SHA256_DIGEST_SIZE)
#define HTC_CRYPTO_SHA256_DIGEST_SIZE 32 /* SHA-256 digest size */
#endif

#if !defined(HTC_CRYPTO_USE_DOUBLE) || HTC_CRYPTO_USE_DOUBLE != 0
typedef float htc_crypto_float_t; /* Use float by default */
#else
typedef double htc_crypto_float_t; /* Use double if HTC_CRYPTO_USE_DOUBLE is set */
#endif

/* Platform */
#if !defined(HTC_CRYPTO_LANGUAGE_C)
#define HTC_CRYPTO_LANGUAGE_C 0
#endif

#if !defined(HTC_CRYPTO_LANGUAGE_CPP)
#define HTC_CRYPTO_LANGUAGE_CPP 0
#endif

#if !defined(HTC_CRYPTO_PLATFORM_ANDROID)
#define HTC_CRYPTO_PLATFORM_ANDROID 0
#endif

#if !defined(HTC_CRYPTO_PLATFORM_LINUX)
#define HTC_CRYPTO_PLATFORM_LINUX 0
#endif

#if !defined(HTC_CRYPTO_PLATFORM_WINDOWS)
#define HTC_CRYPTO_PLATFORM_WINDOWS 0
#endif

#if !defined(HTC_CRYPTO_PLATFORM_UNSUPPORTED)
#define HTC_CRYPTO_PLATFORM_UNSUPPORTED 0
#endif

#if !defined(__cplusplus)
#undef HTC_CRYPTO_LANGUAGE_C
#define HTC_CRYPTO_LANGUAGE_C 1
#else
#undef HTC_CRYPTO_LANGUAGE_CPP
#define HTC_CRYPTO_LANGUAGE_CPP 1
#endif

#if defined(__ANDROID__)
#undef HTC_CRYPTO_PLATFORM_ANDROID
#define HTC_CRYPTO_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef HTC_CRYPTO_PLATFORM_LINUX
#define HTC_CRYPTO_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef HTC_CRYPTO_PLATFORM_WINDOWS
#define HTC_CRYPTO_PLATFORM_WINDOWS 1
#else
#undef HTC_CRYPTO_PLATFORM_UNSUPPORTED
#define HTC_CRYPTO_PLATFORM_UNSUPPORTED 1
#endif

#if HTC_CRYPTO_PLATFORM_ANDROID || HTC_CRYPTO_PLATFORM_LINUX
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
#if HTC_CRYPTO_HAVE_STDBOOL > 0
#include <stdbool.h> /* For bool type */
#endif

#if HTC_CRYPTO_HAVE_STDDEF > 0
#include <stddef.h> /* For size_t */
#endif

#if HTC_CRYPTO_HAVE_STDINT > 0
#include <stdint.h> /* For sized int types */
#endif

/* Macros */
#if !defined(HTC_CRYPTO_UNUSED)
#define HTC_CRYPTO_UNUSED(unused) (void)(true ? (void)0 : ((void)(unused)));
#endif

/* Allocator API */
typedef struct htc_crypto_allocator_o {
	uint32_t opaque[8];
} htc_crypto_allocator_o;

typedef struct htc_crypto_allocator_i {
	htc_crypto_allocator_o o;
	void *(*realloc)(struct htc_crypto_allocator_i *a, void *ptr, size_t old_size, size_t new_size);
} htc_crypto_allocator_i;

/* Base64 API */
HTC_CRYPTO_EXPORT uint8_t *htc_base64_decode(const char *encoded, htc_crypto_allocator_i *allocator);

HTC_CRYPTO_EXPORT char *htc_base64_encode(const uint8_t *buffer, size_t bytes, htc_crypto_allocator_i *allocator);

/* Hex API */
HTC_CRYPTO_EXPORT char *htc_crypto_hex_string(uint8_t *data, size_t size, htc_crypto_allocator_i *allocator);

/* SHA256 API */
typedef struct htc_sha256_t {
	uint64_t bit_len;
	uint8_t data[64];
	size_t data_len;
	uint32_t state[8];
} htc_sha256_t;

HTC_CRYPTO_EXPORT bool htc_sha256_final(htc_sha256_t *s, uint8_t *digest);

HTC_CRYPTO_EXPORT bool htc_sha256_init(htc_sha256_t *s);

HTC_CRYPTO_EXPORT bool htc_sha256_update(htc_sha256_t *s, const uint8_t *data, size_t size);

#if defined(__cplusplus)
}
#endif

#endif

/* Source */
#if defined(HTC_CRYPTO_IMPLEMENTATION)
#undef HTC_CRYPTO_IMPLEMENTATION

#include <stdio.h>  /* For sprintf */
#include <string.h> /* For strlen */

/* Base64 API */
uint8_t *htc_base64_decode(const char *encoded, htc_crypto_allocator_i *allocator) {
	uint32_t i = 0;
	uint8_t *input = (uint8_t *)encoded;
	uint8_t *ret = NULL;
	size_t size = 0;

	static const int base64_decode[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	                              -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62,
	                              -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, 0,  -1, -1, -1, 0,
	                              1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
	                              23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	                              39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1};

	if (allocator == NULL || encoded == NULL) {
		return NULL;
	}

	size = ((strlen(encoded) - 1) * 3) / 4 + 1;

	ret = (uint8_t *)allocator->realloc(allocator, NULL, 0, size);

	memset(ret, 0, size);

	do {
		ret[i] = base64_decode[input[0]] << 2 | (base64_decode[input[1]] & 0x30) >> 4;
		i += 1;

		if (input[2] != '=') {
			ret[i] = (base64_decode[input[1]] & 0x0F) << 4 | (base64_decode[input[2]] & 0x3C) >> 2;

			i += 1;
		}

		if (input[3] != '=') {
			ret[i] = (base64_decode[input[2]] & 0x03) << 6 | base64_decode[input[3]];

			i += 1;
		}

		input += 4;
	} while (size -= 3);

	return ret;
}

char *htc_base64_encode(const uint8_t *buffer, size_t bytes, htc_crypto_allocator_i *allocator) {
	static const char *base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned int i = 0;
	unsigned int j = 0;
	uint8_t k = 0;
	size_t pos = 0;
	char *ret = NULL;
	size_t size = 0;

	if (allocator == NULL || buffer == NULL || bytes == 0) {
		return NULL;
	}

	size = ((bytes + 2) / 3) * 4 + 1;

	ret = (char *)allocator->realloc(allocator, NULL, 0, size);

	for (i = 0; i < bytes; i++) {
		switch (pos) {
		case 0:
			pos = 1;
			ret[j++] = base64[(buffer[i] >> 2) & 0x3F];

			break;
		case 1:
			pos = 2;
			ret[j++] = base64[((k & 0x3) << 4) | ((buffer[i] >> 4) & 0xF)];

			break;
		case 2:
			pos = 0;
			ret[j++] = base64[((k & 0xF) << 2) | ((buffer[i] >> 6) & 0x3)];
			ret[j++] = base64[buffer[i] & 0x3F];

			break;
		}

		k = buffer[i];
	}

	switch (pos) {
	case 1:
		ret[j++] = base64[(k & 0x3) << 4];
		ret[j++] = '=';
		ret[j++] = '=';

		break;
	case 2:
		ret[j++] = base64[(k & 0xF) << 2];
		ret[j++] = '=';

		break;
	}

	ret[j] = 0;

	return ret;
}

/* Hex API */
char *htc_crypto_hex_string(uint8_t *data, size_t size, htc_crypto_allocator_i *allocator) {
	size_t i = 0;
	char *ptr = NULL;

	if (data == NULL || size == 0 || allocator == NULL) {
		return NULL;
	}

	ptr = allocator->realloc(allocator, NULL, 0, (size * 2) + 1);

	if (ptr == NULL) {
		return NULL;
	}

	for (i = 0; i < size; i++) {
		sprintf(&(ptr[i * 2]), "%02x", data[i]);
	}

	ptr[size * 2] = 0;

	return ptr;
}

/* SHA256 API */
#if !defined(HTC_CRYPTO_ROTR)
#define HTC_CRYPTO_ROTR(a, b) (((a) >> (b)) | ((a) << (32 - (b))))
#endif

#if !defined(HTC_CRYPTO_CH)
#define HTC_CRYPTO_CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#endif

#if !defined(HTC_CRYPTO_MAJ)
#define HTC_CRYPTO_MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#endif

#if !defined(HTC_CRYPTO_EP0)
#define HTC_CRYPTO_EP0(x) (HTC_CRYPTO_ROTR(x, 2) ^ HTC_CRYPTO_ROTR(x, 13) ^ HTC_CRYPTO_ROTR(x, 22))
#endif

#if !defined(HTC_CRYPTO_EP1)
#define HTC_CRYPTO_EP1(x) (HTC_CRYPTO_ROTR(x, 6) ^ HTC_CRYPTO_ROTR(x, 11) ^ HTC_CRYPTO_ROTR(x, 25))
#endif

#if !defined(HTC_CRYPTO_SIG0)
#define HTC_CRYPTO_SIG0(x) (HTC_CRYPTO_ROTR(x, 7) ^ HTC_CRYPTO_ROTR(x, 18) ^ ((x) >> 3))
#endif

#if !defined(HTC_CRYPTO_SIG1)
#define HTC_CRYPTO_SIG1(x) (HTC_CRYPTO_ROTR(x, 17) ^ HTC_CRYPTO_ROTR(x, 19) ^ ((x) >> 10))
#endif

void htc__sha256_transform(htc_sha256_t *s, const uint8_t data[]);

bool htc_sha256_final(htc_sha256_t *s, uint8_t *digest) {
	uint32_t i;

	if (s == NULL || digest == NULL) {
		return false;
	}

	i = s->data_len;

	if (s->data_len < 56) {
		s->data[i++] = 0x80;

		while (i < 56) {
			s->data[i++] = 0x00;
		}
	} else {
		s->data[i++] = 0x80;

		while (i < 64) {
			s->data[i++] = 0x00;
		}

		htc__sha256_transform(s, s->data);

		memset(s->data, 0, 56);
	}

	s->bit_len += s->data_len * 8;
	s->data[63] = s->bit_len;
	s->data[62] = s->bit_len >> 8;
	s->data[61] = s->bit_len >> 16;
	s->data[60] = s->bit_len >> 24;
	s->data[59] = s->bit_len >> 32;
	s->data[58] = s->bit_len >> 40;
	s->data[57] = s->bit_len >> 48;
	s->data[56] = s->bit_len >> 56;

	htc__sha256_transform(s, s->data);

	for (i = 0; i < 4; i++) {
		digest[i] = (s->state[0] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 4] = (s->state[1] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 8] = (s->state[2] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 12] = (s->state[3] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 16] = (s->state[4] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 20] = (s->state[5] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 24] = (s->state[6] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 28] = (s->state[7] >> (24 - i * 8)) & 0x000000ff;
	}

	htc_sha256_init(s);

	return true;
}

bool htc_sha256_init(htc_sha256_t *s) {
	if (s == NULL) {
		return false;
	}

	s->state[0] = 0x6a09e667;
	s->state[1] = 0xbb67ae85;
	s->state[2] = 0x3c6ef372;
	s->state[3] = 0xa54ff53a;
	s->state[4] = 0x510e527f;
	s->state[5] = 0x9b05688c;
	s->state[6] = 0x1f83d9ab;
	s->state[7] = 0x5be0cd19;

	s->bit_len = 0;
	s->data_len = 0;

	return true;
}

void htc__sha256_transform(htc_sha256_t *s, const uint8_t data[]) {
	uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	static const uint32_t K[64] = {
	    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

	for (i = 0, j = 0; i < 16; i++, j += 4) {
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	}

	for (; i < 64; i++) {
		m[i] = HTC_CRYPTO_SIG1(m[i - 2]) + m[i - 7] + HTC_CRYPTO_SIG0(m[i - 15]) + m[i - 16];
	}

	a = s->state[0];
	b = s->state[1];
	c = s->state[2];
	d = s->state[3];
	e = s->state[4];
	f = s->state[5];
	g = s->state[6];
	h = s->state[7];

	for (i = 0; i < 64; i++) {
		t1 = h + HTC_CRYPTO_EP1(e) + HTC_CRYPTO_CH(e, f, g) + K[i] + m[i];
		t2 = HTC_CRYPTO_EP0(a) + HTC_CRYPTO_MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	s->state[0] += a;
	s->state[1] += b;
	s->state[2] += c;
	s->state[3] += d;
	s->state[4] += e;
	s->state[5] += f;
	s->state[6] += g;
	s->state[7] += h;
}

bool htc_sha256_update(htc_sha256_t *s, const uint8_t *data, size_t size) {
	uint32_t i;

	if (s == NULL || data == NULL || size == 0) {
		return false;
	}

	for (i = 0; i < size; i++) {
		s->data[s->data_len] = data[i];

		s->data_len++;

		if (s->data_len == 64) {
			htc__sha256_transform(s, s->data);

			s->bit_len += 512;

			s->data_len = 0;
		}
	}

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
