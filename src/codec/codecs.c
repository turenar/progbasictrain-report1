#include "config.inc.h"

#include "codecs.h"
#include <string.h>

typedef struct {
	const char* name;
	pbmcodec_reader_fn fn;
} pbmcodec_reader_info;
typedef struct {
	const char* name;
	pbmcodec_writer_fn fn;
} pbmcodec_writer_info;

static pbmcodec_reader_info readers[] = {
		{"pbm", pbmcodec_pbm_read},
		{"png", pbmcodec_png_read},
		{NULL, NULL}
};

static pbmcodec_writer_info writers[] = {
		{"pbm", pbmcodec_pbm_write},
		{"png", pbmcodec_png_write},
		{NULL, NULL}
};


pbmcodec_reader_fn pbmcodec_get_reader(const char* name) {
	for (const pbmcodec_reader_info* p = readers; p->name != NULL; p++) {
		if (strcmp(name, p->name) == 0) {
			return p->fn;
		}
	}
	return NULL;
}

pbmcodec_writer_fn pbmcodec_get_writer(const char* name) {
	for (const pbmcodec_writer_info* p = writers; p->name != NULL; p++) {
		if (strcmp(name, p->name) == 0) {
			return p->fn;
		}
	}
	return NULL;
}

