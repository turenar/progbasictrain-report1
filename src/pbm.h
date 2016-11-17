#pragma once

#include <stdint.h>

typedef struct pbm_info_tag{
	int width;
	int height;
	uint8_t** data;
} pbm_info;

typedef enum pbm_error_tag{
	PBM_SUCCESS = 0,
	PBMCODEC_INVALID_SIGNATURE,
	PBMCODEC_INVALID_HEADER,
	PBMCODEC_INVALID_DATA,
	PBM_ALLOCATION_FAILED,
	PBM_SYSTEM_ERROR,
	PBMFILTER_INVALID_ARG,
	PBMFILTER_NOT_FOUND
} pbm_error_t;

void pbm_init(pbm_info*);
void pbm_resize(pbm_info*, int width, int height);
void pbm_free(pbm_info*);
void pbm_copy(const pbm_info*, pbm_info*);
