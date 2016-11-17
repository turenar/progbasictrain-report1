#include "config.inc.h"

#include "filters.h"
#include "logger.h"

static pbm_info diff_original = {0, 0, NULL};

pbm_error_t pbmfilter_diff1(const pbm_info* in, pbm_info* out, char** args) {
	UNUSED_VAR(args);
	if (diff_original.data != NULL) {
		LOG(error, "filter diff1 can be used only once before diff2");
		return PBMFILTER_INVALID_ARG;
	}
	pbm_init(&diff_original);
	pbm_copy(in, &diff_original);
	pbm_copy(in, out);
	return PBM_SUCCESS;
}

pbm_error_t pbmfilter_diff2(const pbm_info* in, pbm_info* out, char** args) {
	UNUSED_VAR(args);
	if (diff_original.data == NULL) {
		LOG(error, "filter diff2 can be used only once after diff1");
		return PBMFILTER_INVALID_ARG;
	}
	if (in->width != diff_original.width || in->height != diff_original.height) {
		LOG(error, "diff2: size mismatch; expected=%dx%d, actual=%dx%d",
		    diff_original.width, diff_original.height, in->width, in->height);
		return PBMFILTER_INVALID_ARG;
	}

	pbm_resize(out, in->width, in->height);

	uint8_t** orig_row_p = diff_original.data;
	uint8_t** in_row_p = in->data;
	uint8_t** out_row_p = out->data;
	for (int y = 0; y < diff_original.height; ++y) {
		uint8_t* orig_col_p = *orig_row_p++;
		uint8_t* in_col_p = *in_row_p++;
		uint8_t* out_col_p = *out_row_p++;
		for (int x = 0; x < diff_original.width; ++x) {
			*out_col_p++ = (uint8_t) (*orig_col_p++ != *in_col_p++);
		}
	}
	pbm_free(&diff_original);
	return PBM_SUCCESS;
}

