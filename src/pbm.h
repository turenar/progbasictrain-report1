#pragma once

#include <stdio.h>
#include <stdint.h>

typedef struct {
	int width;
	int height;
	uint8_t** data;
} pbm_info;

typedef enum {
	PBM_SUCCESS = 0,
	PBM_INVALID_HEADER,
	PBM_LACK_DATA,
	PBM_INVALID_DATA,
	PBM_SYSTEM_ERROR,
	PBMFILTER_INVALID_ARG,
} pbm_error_t;

void pbm_init(pbm_info*, int width, int height);
void pbm_free(pbm_info*);
pbm_error_t pbm_load(pbm_info*, FILE*);
void pbm_write(const pbm_info*, FILE*);
