#include <pbm.h>
#include "config.inc.h"

#include "filters.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

#define ICM_DEFAULT_BETA 1
#define ICM_DEFAULT_GAMMA 2
#define ICM_DEFAULT_RADIUS 1
#define ICM_DEFAULT_WEIGHT_METHOD ICM_WEIGHT_FLAT
#define ICM_LOOP_LIMIT 100

typedef enum {
	ICM_WEIGHT_FLAT = 0, ICM_WEIGHT_SQUARE, ICM_WEIGHT_DIA, ICM_WEIGHT_CIRCLE,
	ICM_WEIGHT_INVALID = -1
} icm_weight_method;

typedef struct icm_param_tag {
	int beta;
	int gamma;
	int search_radius;
	icm_weight_method type;
} icm_param;

typedef struct {
	char* name;
	icm_weight_method method;
} name_method_map_t;
name_method_map_t name_method_map[] = {
		{"flat",   ICM_WEIGHT_FLAT},
		{"square", ICM_WEIGHT_SQUARE},
		{"dia",    ICM_WEIGHT_DIA},
		{"circle", ICM_WEIGHT_CIRCLE},
		{NULL, ICM_DEFAULT_WEIGHT_METHOD}};

static pbm_error_t parse_args(char** args, icm_param* param);
static uint8_t search_lower_energy_value(const pbm_info*, icm_param, int x, int y);
static int get_energy_with_value(const pbm_info*, icm_param, int x, int y, int value);
static double get_mag_from_param(icm_param, int dx, int dy);
static int fit_position(int v, int max);

pbm_error_t pbmfilter_icm2(const pbm_info* in, pbm_info* out, char** args) {
	UNUSED_VAR(args);

	icm_param param = {ICM_DEFAULT_BETA, ICM_DEFAULT_GAMMA, ICM_DEFAULT_RADIUS, ICM_DEFAULT_WEIGHT_METHOD};
	pbm_error_t arg_result = parse_args(args, &param);
	if (arg_result) {
		return arg_result;
	}

	pbm_resize(out, in->width, in->height);
	pbm_copy(in, out);

	int updated;
	int loop_limit = ICM_LOOP_LIMIT;
	do {
		updated = 0;
		uint8_t** row_p = out->data;
		for (int y = 0; y < in->height; ++y) {
			uint8_t* col_p = *row_p++;
			for (int x = 0; x < in->width; ++x) {
				uint8_t lower_energy_value = search_lower_energy_value(out, param, x, y);
				if (*col_p != lower_energy_value) {
					updated++;
					*col_p = lower_energy_value;
				}
				col_p++;
			}
		}
	} while (--loop_limit > 0 && updated > 16); //TODO

	return PBM_SUCCESS;
}

static pbm_error_t parse_args(char** args, icm_param* param) {
	for (char** p = args; *p != NULL; p++) {
		char* arg = *p;
		if (arg[0] == '\0') {
			LOG(error, "invalid arg: '(null)'");
			return PBMFILTER_INVALID_ARG;
		} else if (arg[1] != '=') {
			LOG(error, "invalid arg: '%s'", arg);
			return PBMFILTER_INVALID_ARG;
		}
		char* opt = arg + 2;
		char* endptr = NULL;
		switch (arg[0]) {
			case 'b': {
				int b = (int) strtol(opt, &endptr, 10);
				param->beta = b;
				break;
			}
			case 'g': {
				int g = (int) strtol(opt, &endptr, 10);
				param->gamma = g;
				break;
			}
			case 'r': {
				int r = (int) strtol(opt, &endptr, 10);
				param->search_radius = r;
				break;
			}
			case 't': {
				icm_weight_method w = ICM_WEIGHT_INVALID;
				for (name_method_map_t* nm = name_method_map; nm->name != NULL; nm++) {
					if (strcmp(opt, nm->name) == 0) {
						w = nm->method;
					}
				}
				if (w == ICM_WEIGHT_INVALID) {
					LOG(error, "invalid method name: '%s'", opt);
					return PBMFILTER_INVALID_ARG;
				} else {
					param->type = w;
				}
				break;
			}
			default:
				LOG(error, "invalid argument: %s", arg);
				return PBMFILTER_INVALID_ARG;
		}
		if (endptr != NULL && *endptr != '\0') {
			LOG(error, "invalid number: '%s'", arg);
			return PBMFILTER_INVALID_ARG;
		}
	}
	return PBM_SUCCESS;
}

static uint8_t search_lower_energy_value(const pbm_info* info, icm_param param, int x, int y) {
	uint8_t orig = info->data[y][x];
	int with0 = get_energy_with_value(info, param, x, y, 0);
	int with1 = get_energy_with_value(info, param, x, y, 1);

	if (with0 == with1) {
		return orig;
	} else if (with0 < with1) {
		return 0;
	} else {
		return 1;
	}
}

static int get_energy_with_value(const pbm_info* info, icm_param param, int x, int y, int value) {
	double energy = 0;

	for (int dy = -param.search_radius; dy <= param.search_radius; ++dy) {
		int row = fit_position(y + dy, info->height);
		uint8_t* row_p = info->data[row];
		for (int dx = -param.search_radius; dx <= param.search_radius; ++dx) {
			if (!(dy == 0 && dx == 0)) {
				energy -= get_mag_from_param(param, dx, dy)
				          * (2 * row_p[fit_position(x + dx, info->width)] - 1) * (2 * value - 1);
			}
		}
	}

	energy -= param.gamma * (2 * info->data[y][x] - 1) * (2 * value - 1);
	return (int) energy;
}

static int fit_position(int v, int max) {
	if (v < 0 || v >= max) {
		v = 0;
	}
	return v;
}

static double get_mag_from_param(icm_param param, int dx, int dy) {
	dx = abs(dx);
	dy = abs(dy);
	switch (param.type) {
		case ICM_WEIGHT_FLAT:
			return param.beta;
		case ICM_WEIGHT_SQUARE:
			return param.beta / (double) (dx > dy ? dx : dy);
		case ICM_WEIGHT_DIA:
			return param.beta / (double) (dx + dy);
		case ICM_WEIGHT_CIRCLE:
			return param.beta / sqrt(dx * dx + dy * dy);
		default:
			LOG(error, "fatal logic error");
			abort();
	}
}
