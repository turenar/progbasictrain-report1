#pragma once

#include <stdint.h>

typedef struct {
	int width;
	int height;
	uint8_t** data;
} pbm_info;

typedef enum {
	PBM_SUCCESS = 0,
	PBMCODEC_INVALID_SIGNATURE,
	PBMCODEC_INVALID_HEADER,
	PBMCODEC_INVALID_DATA,
	PBM_SYSTEM_ERROR,
	PBMFILTER_INVALID_ARG,
} pbm_error_t;

void pbm_init(pbm_info*, int width, int height);
void pbm_free(pbm_info*);
void pbm_copy(const pbm_info*, pbm_info*);