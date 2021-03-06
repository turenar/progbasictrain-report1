#include "config.inc.h"

#include "pbm.h"
#include <stdlib.h>
#include <string.h>

void pbm_init(pbm_info*info){
	info->height = -1;
	info->width = -1;
	info->data = NULL;
}

pbm_error_t pbm_resize(pbm_info* info, int width, int height) {
	if (info->data != NULL) {
		pbm_free(info);
	}
	uint8_t** data = (uint8_t**) malloc(sizeof(uint8_t*) * (size_t) height);
	if (data == NULL) {
		return PBM_ALLOCATION_FAILED;
	}

	uint8_t** p = data;
	pbm_error_t result;
	for (int y = 0; y < height; ++y) {
		uint8_t* q = (uint8_t*) malloc(sizeof(uint8_t) * (size_t) width);
		if (q == NULL) {
			result = PBM_ALLOCATION_FAILED;
			goto error;
		}
		*p++ = q;
	}

	info->width = width;
	info->height = height;
	info->data = data;
	return PBM_SUCCESS;

error:
	p = data;
	for (int y = 0; y < height; ++y) {
		uint8_t* q = *p++;
		if (q == NULL) {
			break;
		}
		free(q);
	}
	free(p);
	return result;
}

void pbm_free(pbm_info* info) {
	if (info->data == NULL) {
		return;
	}
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
	pbm_resize(out, in->width, in->height);

	uint8_t** in_p = in->data;
	uint8_t** out_p = out->data;
	for (int y = 0; y < in->height; ++y) {
		memcpy(*out_p++, *in_p++, sizeof(uint8_t) * (size_t) in->width);
	}
}
