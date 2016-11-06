

#include "filter_common.h"
#include <assert.h>

void pbmfilter_apply(const pbm_info* in, pbm_info* out, pbmfilter_p2p_t filter, void* extra) {
	assert(in->height == out->height);
	assert(in->width == out->width);

	uint8_t** read_row_p = in->data;
	uint8_t** write_row_p = out->data;
	for (int y = 0; y < in->height; ++y) {
		uint8_t* read_col_p = *read_row_p++;
		uint8_t* write_col_p = *write_row_p++;
		for (int x = 0; x < in->width; ++x) {
			*write_col_p++ = filter(*read_col_p++, extra);
		}
	}
}
