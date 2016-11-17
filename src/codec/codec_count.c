#include "config.inc.h"

#include "codecs.h"

pbm_error_t pbmcodec_count(const pbm_info* info, FILE* fp) {
	int count = 0;
	uint8_t** row_p = info->data;
	for (int y = 0; y < info->height; ++y) {
		uint8_t* col_p = *row_p++;
		for (int x = 0; x < info->width; ++x) {
			count += *col_p++;
		}
	}

	fprintf(fp, "%d\n", count);
	return PBM_SUCCESS;
}
