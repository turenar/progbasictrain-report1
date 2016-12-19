#pragma once

#include "pbm.h"

typedef uint8_t(* pbmfilter_p2p_t)(uint8_t, void*);

/**
 * pixel to pixelなフィルタリングをするユーティリティー関数
 *
 * 普通に直書きしたほうが最適化されそう
 * @param in 入力
 * @param out 出力
 * @param filter 入力ピクセルから出力ピクセルを生成する関数
 * @param data filterに渡すエキストラデータ
 */
void pbmfilter_apply(const pbm_info* in, pbm_info* out, pbmfilter_p2p_t filter, void* data);
