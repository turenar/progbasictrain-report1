#include "config.inc.h"

#include "pbm.h"
#include <stdlib.h>
#include <string.h>

void pbm_init(pbm_info* info, int width, int height) {
	uint8_t** data = (uint8_t**) malloc(sizeof(uint8_t*) * (size_t) height);
	uint8_t** p = data;
	for (int y = 0; y < height; ++y) {
		*p++ = (uint8_t*) malloc(sizeof(uint8_t) * (size_t) width);
	}

	info->width = width;
	info->height = height;
	info->data = data;
}

void pbm_free(pbm_info* info) {
	uint8_t** p = info->data;
	for (int y = 0; y < info->height; ++y) {
		free(*p++);
	}
	free(info->data);
	info->height = -1;
	info->width = -1;
	info->data = NULL;
}

void pbm_copy(const pbm_info* in, pbm_info* out) {
	pbm_init(out, in->width, in->height);

	uint8_t** in_p = in->data;
	uint8_t** out_p = out->data;
	for (int y = 0; y < in->height; ++y) {
		memcpy(*out_p++, *in_p++, sizeof(uint8_t) * (size_t) in->width);
	}
}
