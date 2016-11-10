#include "config.inc.h"

#include "codecs.h"
#include <stdlib.h>

static int _pbm_check_file_signature(const char*);
static pbm_error_t _pbm_extract_size_from_header(const char*, pbm_info*);


pbm_error_t codec_pbm_read(pbm_info* info, FILE* fp) {
	char buf[65536];
	char* res;

	// line 1: P1 header
	res = fgets(buf, sizeof(buf), fp);
	if (res == NULL || !_pbm_check_file_signature(buf)) {
		return PBM_INVALID_HEADER;
	}
	// line 2: width, height
	res = fgets(buf, sizeof(buf), fp);
	if (res == NULL) {
		return PBM_INVALID_HEADER;
	}
	int ret = _pbm_extract_size_from_header(buf, info);
	if (ret) {
		return PBM_INVALID_HEADER;
	}

	pbm_init(info, info->width, info->height);
	// image data...
	uint8_t** row_p = info->data;
	for (int y = 0; y < info->height; ++y) {
		res = fgets(buf, sizeof(buf), fp);
		if (res == NULL) {
			return PBM_LACK_DATA;
		}

		char* str_p = buf;
		uint8_t* col_p = *row_p++;
		for (int x = 0; x < info->width; ++x) {
			char b = *str_p++;
			if (b == '0' || b == '1') {
				*col_p++ = (uint8_t) (b - '0');
			} else {
				return PBM_INVALID_DATA;
			}
			char delimiter = *str_p++;
			if (delimiter == ' ') {
				continue; // do nothing
			} else if (delimiter == '\0' || delimiter == '\n' || delimiter == '\r') {
				// row end
				if (x + 1 != info->width) {
					return PBM_LACK_DATA;
				} else {
					continue;
				}
			} else {
				return PBM_INVALID_DATA;
			}
		}
	}
	return PBM_SUCCESS;
}

pbm_error_t codec_pbm_write(const pbm_info* info, FILE* fp) {
	fputs("P1\n", fp);
	fprintf(fp, "%d %d\n", info->width, info->height);

	uint8_t** row_p = info->data;
	for (int y = 0; y < info->height; ++y) {
		uint8_t* col_p = *row_p++;
		for (int x = 0; x < info->width; ++x) {
			if (x != 0) {
				fputc(' ', fp);
			}
			fputc('0' + (*col_p++), fp);
		}
		fputc('\n', fp);
	}
	return PBM_SUCCESS;
}

static pbm_error_t _pbm_extract_size_from_header(const char* str, pbm_info* info) {
	long width, height;
	char* endptr;
	width = strtol(str, &endptr, 10);
	if (*endptr++ != ' ') {
		return PBM_INVALID_HEADER;
	}
	height = strtol(endptr, &endptr, 10);
	if (!(*endptr == '\n' || *endptr == '\r')) {
		return PBM_INVALID_HEADER;
	}

	if (width <= 0 || height <= 0) {
		return PBM_INVALID_HEADER;
	}
	info->width = (int) width;
	info->height = (int) height;
	return PBM_SUCCESS;
}

static int _pbm_check_file_signature(const char* signature) {
	return signature[0] == 'P' &&
	       signature[1] == '1' &&
	       (signature[2] == '\n' ||
	        (signature[2] == '\r' && signature[3] == '\n'));
}
