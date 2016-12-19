#pragma once

#include <stdint.h>

/**
 * PBM画像情報
 *
 * あらゆる使用前に全てのメモリ領域を0で初期化するか、 pbm_init(pbm_info*) を呼び出さなくてはいけない
 */
typedef struct pbm_info_tag {
	int width;
	int height;
	/** data[y][x] */
	uint8_t** data;
} pbm_info;

/**
 * エラー情報
 *
 * if(pbm_error_t_var) でエラーが発生したかどうかが確認できる。
 * エラーが発生した場合には、const変数の維持及びpbm_infoが構造的に不整合を起こしていないこと以外は保証されない。
 */
typedef enum pbm_error_tag {
	PBM_SUCCESS = 0, /*!< 成功 */
	PBM_ALLOCATION_FAILED, /*!< メモリ確保に失敗 */
	PBM_SYSTEM_ERROR, /*!< システム/ライブラリエラー */
	PBMCODEC_INVALID_SIGNATURE, /*!< codec: シグネチャが不正 */
	PBMCODEC_INVALID_HEADER, /*!< codec: ヘッダが不正 */
	PBMCODEC_INVALID_DATA, /*!< codec: データが不正 */
	PBMFILTER_INVALID_ARG, /*!< filter: 引数が不正 */
	PBMFILTER_NOT_FOUND /*!< filter: 指定した名前のフィルタが見つからない */
} pbm_error_t;

/*
 * 本当は、pbm_init* pbm_alloc()のほうが良かったのかもしれない
 */
/**
 * pbm_infoを初期化する。0初期化でもいいが、この関数を呼び出すようにしたほうが将来互換性が保たれる
 * (まあ所詮レポート課題なので保つ必要はないが)。
 *
 * 未初期化のpbm_infoについては、pbm_init以外の動作が保証されない。
 *
 * また、すでに初期化されているpbm_infoを初期化したくなった場合は、 pbm_free(pbm_info*) を呼び出すようにする。
 * メモリリークの原因になるので pbm_init(pbm_info*) を呼び出してはならない。
 */
void pbm_init(pbm_info*);
/**
 * 初期化されている pbm_info をとり、必要であればメモリを解放し、指定したサイズの画像を保持するのに十分なメモリ領域を確保する。
 *
 * この関数では元の画像データからリサイズ後の画像データにピクセルコピーは行わない。
 * すでにメモリが解放されている、あるいはメモリが確保されていない場合には、この関数ではメモリ解放を行わない。
 * @param info 初期化済みpbm_info。dataの確保後の内容は不定。
 * @param width 幅
 * @param height 高さ
 * @return pbm_error_t
 */
pbm_error_t pbm_resize(pbm_info*info, int width, int height);
/**
 * 初期化されている pbm_info をとり、必要であればメモリを解放し、pbm_infoを再度初期化する。
 * @param info 一度初期化されているpbm_info
 */
void pbm_free(pbm_info*info);
/**
 * src から dst にサイズおよび画像データをコピーする。
 *
 * @param src 初期化されているpbm_info。
 * @param dst 初期化されているpbm_info。内容は破壊される。
 */
void pbm_copy(const pbm_info*src, pbm_info*dst);
