#include "config.inc.h"

#ifdef USE_LIBPNG

#include "codecs.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "logger.h"

#ifndef PNG_ERROR_ACTION_WARN
#define PNG_ERROR_ACTION_WARN 2
#endif

#define PNG_HEADER_BYTES 8

static void user_warning_fn(png_structp, png_const_charp);
static void user_error_fn(png_structp, png_const_charp);

pbm_error_t pbmcodec_png_read(pbm_info* info, FILE* fp) {
	png_byte header[PNG_HEADER_BYTES];
	if (fread(header, 1, PNG_HEADER_BYTES, fp) < sizeof(PNG_HEADER_BYTES)) {
		LOG(error, "invalid signature");
		return PBMCODEC_INVALID_SIGNATURE;
	}

	int is_png = !png_sig_cmp(header, 0, PNG_HEADER_BYTES);
	if (!is_png) {
		LOG(error, "invalid signature");
		return PBMCODEC_INVALID_SIGNATURE;
	}
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, user_error_fn, user_warning_fn);
	if (!png_ptr) {
		LOG(error, "libpng initialization failed");
		return PBM_SYSTEM_ERROR;
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		LOG(error, "libpng initialization failed");
		return PBM_SYSTEM_ERROR;
	}
	// エラー処理用
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		LOG(error, "libpng failed");
		return PBM_SYSTEM_ERROR;
	}

	png_init_io(png_ptr, fp);
	// 事前にシグネチャを読込確認済なら、ファイル先頭から読み飛ばしているバイト数を知らせる
	png_set_sig_bytes(png_ptr, PNG_HEADER_BYTES);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_STRIP_ALPHA | PNG_TRANSFORM_EXPAND, NULL);

	if (png_get_interlace_type(png_ptr, info_ptr) != PNG_INTERLACE_NONE) {
		LOG(error, "interlaced png not supported");
		return PBMCODEC_INVALID_HEADER;
	}

	pbm_resize(info, (int) png_get_image_width(png_ptr, info_ptr), (int) png_get_image_height(png_ptr, info_ptr));
	uint8_t** row_p = info->data;

	png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		LOG(warn, "converting rgb png to gray");
		png_set_rgb_to_gray(png_ptr, PNG_ERROR_ACTION_WARN, -1, -1);
	}
	assert(png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_GRAY);

	png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	png_bytepp rows = png_get_rows(png_ptr, info_ptr);
	png_byte threshold = (png_byte) (1 << (bit_depth - 1));
	for (int y = 0; y < info->height; ++y) {
		uint8_t* col_p = *row_p++;
		png_bytep row = *rows++;
		for (int x = 0; x < info->width; ++x) {
			// png-grayscale 0: black 255: white (if 8bit)
			// pbm-monochrome 0: white 1: black :-P
			*col_p++ = (uint8_t) (*row++ <= threshold);
		}
	}
	return PBM_SUCCESS;
}

pbm_error_t pbmcodec_png_write(const pbm_info* info, FILE* fp) {
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, user_error_fn, user_warning_fn);
	pbm_error_t result = PBM_SYSTEM_ERROR;

	if (!png_ptr) {
		return PBM_SYSTEM_ERROR;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_write_struct(&png_ptr, NULL);
		return PBM_SYSTEM_ERROR;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return PBM_SYSTEM_ERROR;
	}

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, (png_uint_32) info->width, (png_uint_32) info->height, 8, PNG_COLOR_TYPE_GRAY,
	             PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_bytepp rows = png_malloc(png_ptr, sizeof(png_bytep) * (size_t) info->height);
	if (rows == NULL) {
		goto error;
	}
	png_set_rows(png_ptr, info_ptr, rows);
	memset(rows, 0, sizeof(png_bytep) * (size_t) info->height);
	size_t row_size = sizeof(png_byte) * (size_t) info->width;

	png_bytepp out_pp = rows;
	uint8_t** in_pp = info->data;
	for (int y = 0; y < info->height; y++) {
		if ((*out_pp = png_malloc(png_ptr, row_size)) == NULL) {
			goto error;
		}
		png_bytep out_p = *out_pp++;
		uint8_t* in_p = *in_pp++;
		for (int x = 0; x < info->width; x++) {
			*out_p++ = (png_byte) ((*in_p++) ? 0 : 255); // negate pbm_mono -> png_gray
		}
	}
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	result = PBM_SUCCESS;

	error:
	if (rows != NULL) {
		for (int y = 0; y < info->height; y++) {
			png_free(png_ptr, rows[y]);
		}
		png_free(png_ptr, rows);
	}
	png_destroy_write_struct(&png_ptr, &info_ptr);
	return result;
}

static void user_warning_fn(png_structp png_ptr, png_const_charp msg) {
	UNUSED_VAR(png_ptr);
	LOG(warn, "libpng: %s", msg);
}

static void user_error_fn(png_structp png_ptr, png_const_charp msg) {
	UNUSED_VAR(png_ptr);
	LOG(error, "libpng: %s", msg);

	png_longjmp(png_ptr, 1);
}


#else
typedef int make_iso_compilers_happy; // suppress "ISO C forbids an empty translation unit"
#endif
