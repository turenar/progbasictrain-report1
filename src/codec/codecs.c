#include "config.inc.h"

#include "codecs.h"
#include <string.h>

typedef struct {
	const char* name;
	pbmcodec_reader_fn fn;
	const char* help;
} pbmcodec_reader_info;
typedef struct {
	const char* name;
	pbmcodec_writer_fn fn;
	const char* help;
} pbmcodec_writer_info;

static pbmcodec_reader_info readers[] = {
		{"pbm", pbmcodec_pbm_read, "read from netpbm, bitmap"},
#ifdef USE_LIBPNG
		{"png", pbmcodec_png_read, "read from png file. paletted png is not supported"},
#endif
		{NULL, NULL, NULL}
};

static pbmcodec_writer_info writers[] = {
		{"pbm", pbmcodec_pbm_write, "save as netpbm, bitmap"},
#ifdef USE_LIBPNG
		{"png", pbmcodec_png_write, "save as 8-bit gray-scaled png"},
#endif
#ifdef USE_LIBSIXEL
		{"six", pbmcodec_sixel_write, "alias of sixel"},
		{"sixel", pbmcodec_sixel_write, "show image in supported console with sixel format"},
#endif
#ifdef USE_GTK
		{"gtk", pbmcodec_gtk_write, "show image in display"},
#endif
		{"count", pbmcodec_count, "(for experts) count black pixels and show"},
		{NULL, NULL, NULL}
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

void pbmcodec_show_help(FILE* fp) {
	fprintf(fp, "  input:\n");
	for (const pbmcodec_reader_info* p = readers; p->name != NULL; p++) {
		fprintf(fp, "    %-10s  %s\n", p->name, p->help);
	}
	fprintf(fp, "  output:\n");
	for (const pbmcodec_writer_info* p = writers; p->name != NULL; p++) {
		fprintf(fp, "    %-10s  %s\n", p->name, p->help);
	}
}
