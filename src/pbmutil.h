#pragma once

#include "filter/filters.h"

typedef struct {
	pbmfilter_fn fn; // 関数
	char** args; // [0]: フィルタ名、[1...]: 引数
} filter_info;

/**
 * 指定した文字列をパースして、フィルタ関数と引数をセットにして返す。
 *
 * 解放には pbm_free_filter_info(filter_info*) を使用する
 * @param filter_spec <filter-name>[:<arg>[:<arg>[...]]] 形式
 * @param info フィルタ関数と引数を格納する先の変数
 * @return エラー情報。エラーの場合はinfoに対して pbm_free_filter_info(filter_info*) する必要はない。
 */
pbm_error_t pbm_parse_filter_str(const char* filter_spec, filter_info* info);
/**
 * pbm_parse_filter_str(const char*, filter_info*) で確保された領域を解放する
 * @return エラー情報
 */
pbm_error_t pbm_free_filter_info(filter_info*);
