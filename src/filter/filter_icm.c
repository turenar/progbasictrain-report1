#include <pbm.h>
#include "config.inc.h"

#include "filters.h"

#define ICM_BETA 1
#define ICM_GAMMA 2
#define ICM_RADIUS 1
#define ICM_PATIENCE 16

/**
 * 指定した座標で、エネルギー関数が小さくなるような値を返す
 * @return エネルギー関数が小さい値
 */
static uint8_t search_lower_energy_value(const pbm_info*, int x, int y);
/**
 * 指定した座標が指定された値だった場合のエネルギー値を返す
 * @return エネルギー値
 */
static int get_energy_with_value(const pbm_info*, int x, int y, int value);
/**
 * 座標として使用できる値にクリップする。
 * 
 * @param v クリップする値。-max <= v < 2*max
 * @param exclusive_max 最大値 (この値は返り値に含まれない)
 * @return 0 <= ret < max
 */
static int fit_position(int v, int exclusive_max);

pbm_error_t pbmfilter_icm(const pbm_info* in, pbm_info* out, char** args) {
	UNUSED_VAR(args);

	pbm_resize(out, in->width, in->height);
	pbm_copy(in, out);

	int updated;
	do {
		updated = 0;
		uint8_t** row_p = out->data;
		for (int y = 0; y < in->height; ++y) {
			uint8_t* col_p = *row_p++;
			for (int x = 0; x < in->width; ++x) {
				uint8_t lower_energy_value = search_lower_energy_value(out, x, y);
				if (*col_p != lower_energy_value) {
					updated++;
					*col_p = lower_energy_value;
				}
				col_p++;
			}
		}
	} while (updated > ICM_PATIENCE); //TODO

	return PBM_SUCCESS;
}

static uint8_t search_lower_energy_value(const pbm_info* info, int x, int y) {
	uint8_t orig = info->data[y][x];
	int with0 = get_energy_with_value(info, x, y, 0);
	int with1 = get_energy_with_value(info, x, y, 1);

	if (with0 == with1) {
		return orig;
	} else if (with0 < with1) {
		return 0;
	} else {
		return 1;
	}
}

static int get_energy_with_value(const pbm_info* info, int x, int y, int value) {
	int energy = 0;

	for (int i = -ICM_RADIUS; i <= ICM_RADIUS; ++i) {
		int row = fit_position(y + i, info->height);
		uint8_t* row_p = info->data[row];
		for (int j = -ICM_RADIUS; j <= ICM_RADIUS; ++j) {
			if (!(i == 0 && j == 0)) {
				energy -= ICM_BETA * (2 * row_p[fit_position(x + j, info->width)] - 1) * (2 * value - 1);
			}
		}
	}

	energy -= ICM_GAMMA * (2 * info->data[y][x] - 1) * (2 * value - 1);
	return energy;
}

static int fit_position(int v, int exclusive_max) {
	if (v < 0) {
		return 0;
	} else if (v >= exclusive_max) {
		return exclusive_max - 1;
	} else {
		return v;
	}
}
