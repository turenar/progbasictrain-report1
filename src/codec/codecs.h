#pragma once

#include <stdio.h>
#include "../pbm.h"

/**
 * FILE*からpbm_infoを作成する関数。
 *
 * PBM_SUCCESS でなければ、infoの中身の値は保証されない。
 */
typedef pbm_error_t (* pbmcodec_reader_fn)(pbm_info*info, FILE*);
/**
 * pbm_infoからFILE*に書き出す関数。
 *
 * 実際にはFILE*を使用する必要はない (GUI系コーデック等)
 */
typedef pbm_error_t (* pbmcodec_writer_fn)(const pbm_info*, FILE*);

/**
 * 名前からreader関数を取得する
 * @param name codec名
 * @return 関数。関連付けられたreaderがなければNULL。
 */
pbmcodec_reader_fn pbmcodec_get_reader(const char* name);
/**
 * 名前からwriter関数を取得する
 * @param name codec名
 * @return 関数。関連付けられたwriterがなければNULL。
 */
pbmcodec_writer_fn pbmcodec_get_writer(const char* name);
/**
 * 指定したfpにヘルプを出力する。
 *
 * 個々のコーデックのヘルプも出力する
 */
void pbmcodec_show_help(FILE* fp);

// PBM
pbm_error_t pbmcodec_pbm_read(pbm_info*, FILE*);
pbm_error_t pbmcodec_pbm_write(const pbm_info*, FILE*);

// 特殊: 1 (黒) のピクセルを数えて数値として出力する
pbm_error_t pbmcodec_count(const pbm_info*, FILE*);

#ifdef USE_LIBPNG
pbm_error_t pbmcodec_png_read(pbm_info*, FILE*);
pbm_error_t pbmcodec_png_write(const pbm_info*, FILE*);
#endif

#ifdef USE_LIBSIXEL
// sixelフォーマットで読むのは諦めた (やる意味が薄い)
//pbm_error_t pbmcodec_sixel_read(pbm_info*, FILE*);
pbm_error_t pbmcodec_sixel_write(const pbm_info*, FILE*);
#endif

#ifdef USE_GTK
pbm_error_t pbmcodec_gtk_write(const pbm_info*, FILE*);
#endif

#ifdef USE_OPENCV
pbm_error_t pbmcodec_cv_write(const pbm_info*, FILE*);
#endif
