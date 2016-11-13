#include <pbm.h>
#include "config.inc.h"

#include "filters.h"

#define ICM_BETA 1
#define ICM_GAMMA 5
#define ICM_NEAR_SIZE 1

static uint8_t search_lower_energy_value(const pbm_info*, int x, int y);
static int get_energy_with_value(const pbm_info*, int x, int y, int value);
static int fit_position(int v, int max);

pbm_error_t pbmfilter_icm(const pbm_info* in, pbm_info* out, char** args) {
	UNUSED_VAR(args);

	pbm_init(out, in->width, in->height);
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
	} while (updated > 16); //TODO

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

	for (int i = -ICM_NEAR_SIZE; i <= ICM_NEAR_SIZE; ++i) {
		int row = fit_position(y + i, info->height);
		uint8_t* row_p = info->data[row];
		for (int j = -ICM_NEAR_SIZE; j <= ICM_NEAR_SIZE; ++j) {
			if (!(i == 0 && j == 0)) {
				energy -= ICM_BETA * (2 * row_p[fit_position(x + j, info->width)] - 1) * (2 * value - 1);
			}
		}
	}

	energy -= ICM_GAMMA * (2 * info->data[y][x] - 1) * (2 * value - 1);
	return energy;
}

static int fit_position(int v, int max) {
	if (v < 0) {
		v += max;
	} else if (v >= max) {
		v -= max;
	}
	return v;
}
