#include "config.inc.h"

#include "codecs.h"
#include <stdlib.h>
#include "logger.h"

/**
 * 指定した文字列がPBMシグネチャであるかどうかをチェックする
 *
 * @return PBMシグネチャであれば1
 */
static int _pbm_check_file_signature(const char*);
/**
 * 指定した文字列から幅及び高さを抜き出す
 * @param size_row ファイル行
 * @param info 幅や高さを格納する先
 * @return 成功すればPBM_SUCCESS
 */
static pbm_error_t _pbm_extract_size_from_header(const char* size_row, pbm_info* info);


pbm_error_t pbmcodec_pbm_read(pbm_info* info, FILE* fp) {
	// PBMだけでなくPPMなどにも対応しても良かったのだけど、
	// PBM系はそもそもあまり使われていないと思うので、、、
	// PNG読み込みコードを流用できれば似非減色もできるはず
	// コメントにも対応していない
	char buf[65536];
	char* res;

	// line 1: P1 header
	res = fgets(buf, sizeof(buf), fp);
	if (res == NULL || !_pbm_check_file_signature(buf)) {
		LOG(error, "invalid signature: %s", buf);
		return PBMCODEC_INVALID_SIGNATURE;
	}

	pbm_free(info); // info.width, heightを_pbm_extract_size_from_headerで破壊するので前もってfreeしておく
	// line 2: width, height
	res = fgets(buf, sizeof(buf), fp);
	if (res == NULL) {
		LOG(error, "unexpected eof");
		return PBMCODEC_INVALID_HEADER;
	}
	pbm_error_t ret = _pbm_extract_size_from_header(buf, info);
	if (ret) {
		return ret;
	}

	pbm_resize(info, info->width, info->height);
	// image data...
	uint8_t** row_p = info->data;
	for (int y = 0; y < info->height; ++y) {
		res = fgets(buf, sizeof(buf), fp);
		if (res == NULL) {
			LOG(error, "unexpected eof [line: %d]", y + 2);
			return PBMCODEC_INVALID_DATA;
		}

		char* str_p = buf;
		uint8_t* col_p = *row_p++;
		for (int x = 0; x < info->width; ++x) {
			// 数字 (0 or 1) の読み込み
			char b = *str_p++;
			if (b == '0' || b == '1') {
				*col_p++ = (uint8_t) (b - '0');
			} else {
				LOG(error, "unexpected char [line: %d, col: %d]: 0x%x", y + 2, 2 * x + 1, b);
				return PBMCODEC_INVALID_DATA;
			}
			// デリミタ (半角スペース) の読み込み
			// タブとかも読み込めるべきなんだろうか。まあ使われてないし……
			// そもそもImageMagickだとPBMはPBMでもバイナリ形式っぽい
			char delimiter = *str_p++;
			if (delimiter == ' ') {
				continue; // do nothing
			} else if (delimiter == '\0' || delimiter == '\n' || delimiter == '\r') {
				// row end
				if (x + 1 != info->width) {
					LOG(error, "width mismatch [line: %d]", y + 2);
					return PBMCODEC_INVALID_DATA;
				} else {
					continue;
				}
			} else {
				LOG(error, "unexpected delimiter [line: %d]: 0x%x", y + 2, delimiter);
				return PBMCODEC_INVALID_DATA;
			}
		}
	}
	return PBM_SUCCESS;
}

pbm_error_t pbmcodec_pbm_write(const pbm_info* info, FILE* fp) {
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
	if (*endptr++ != ' ') { // 数値として読み込めなかったかheightがない
		LOG(error, "invalid size");
		return PBMCODEC_INVALID_HEADER;
	}
	height = strtol(endptr, &endptr, 10);
	if (!(*endptr == '\n' || *endptr == '\r')) { // 行と幅以外に余分なデータが存在する
		LOG(error, "invalid height");
		return PBMCODEC_INVALID_HEADER;
	}

	if (width <= 0 || height <= 0) { // 幅や高さが正しくない
		LOG(error, "invalid size: %ld, %ld", width, height);
		return PBMCODEC_INVALID_HEADER;
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
