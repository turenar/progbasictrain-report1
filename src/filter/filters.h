#pragma once

#include <stdio.h>
#include "../pbm.h"

/**
 * フィルタを適用する関数
 *
 * @param in 入力画像
 * @param out 出力画像。返り値がPBM_SUCCESS出ない場合内容は保証されない (pbm_infoとして正しい状態にはなっている)
 * @param args 引数。最後の要素にはNULLを入れる必要がある
 */
typedef pbm_error_t (* pbmfilter_fn)(const pbm_info* in, pbm_info* out, char** args);

/**
 * 名前からフィルタ関数を取得する。
 * @param name 名前
 * @return 名前と紐付けられたフィルタ関数。紐付けられた関数がなければNULL。
 */
pbmfilter_fn pbmfilter_get_filter(const char* name);
/**
 * 指定したファイルポインタにヘルプを表示する
 */
void pbmfilter_show_help(FILE*);

// ノイズ (args: [probability:default=0.1])
pbm_error_t pbmfilter_noise(const pbm_info* in, pbm_info* out, char** args);
// ノイズ除去 (args: none)
pbm_error_t pbmfilter_icm(const pbm_info* in, pbm_info* out, char** args);
// ノイズ除去 (args: [b=int(=>0)] [g=int(=>0)] [r=int(>0)] [t=string]
pbm_error_t pbmfilter_icm2(const pbm_info* in, pbm_info* out, char** args);
// 差異計算用に保存
pbm_error_t pbmfilter_diff1(const pbm_info* in, pbm_info* out, char** args);
// 差異計算
pbm_error_t pbmfilter_diff2(const pbm_info* in, pbm_info* out, char** args);
