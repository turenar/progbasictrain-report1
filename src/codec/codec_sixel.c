#include "config.inc.h"

#ifdef USE_LIBSIXEL

#include "codecs.h"
#include <stdlib.h>
#include <unistd.h>
#include <sixel.h>
#include "logger.h"

pbm_error_t pbmcodec_sixel_write(const pbm_info* info, FILE* fp) {
	int orig_stdout = dup(STDOUT_FILENO);
	// libsixel は stdout への出力か filename での指定にしか対応していない。fp へ出力させるために stdout を
	// fp の fd で乗っ取る
	dup2(fileno(fp), STDOUT_FILENO);

	unsigned char* data = malloc(sizeof(unsigned char) * (size_t) (info->width * info->height));
	if (!data) {
		LOG(error, "allocation failed");
		return PBM_ALLOCATION_FAILED;
	}

	unsigned char* out_p = data;
	uint8_t** in_row_p = info->data;
	for (int y = 0; y < info->height; ++y) {
		uint8_t* in_col_p = *in_row_p++;
		for (int x = 0; x < info->width; ++x) {
			*out_p++ = (unsigned char) ((*in_col_p++) ? 0 : 255);
		}
	}


	sixel_encoder_t* encoder;
	SIXELSTATUS sixel_status = sixel_encoder_new(&encoder, NULL);
	if (SIXEL_FAILED(sixel_status)) {
		LOG(error, "sixel encoder initialization failed");
		return PBM_ALLOCATION_FAILED;
	}
	sixel_status = sixel_encoder_encode_bytes(encoder, data, info->width, info->height, PIXELFORMAT_G8, NULL, 2);
	if (SIXEL_FAILED(sixel_status)) {
		LOG(error, "sixel encode failed");
		return PBM_SYSTEM_ERROR; // TODO
	}

	dup2(orig_stdout, STDOUT_FILENO);
	return PBM_SUCCESS;
}

#else
typedef int make_iso_compilers_happy; // suppress "ISO C forbids an empty translation unit"
#endif
