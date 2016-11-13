#include "config.inc.h"

#include <assert.h>
#include <stdlib.h>
#include <getopt.h>
#include "filter/filters.h"
#include "codec/codecs.h"
#include "logger.h"


static struct option long_options[] = {
		{"help",     no_argument,       NULL, 'h'},
		{"in-file",  required_argument, NULL, 'i'},
		{"out-file", required_argument, NULL, 'o'},
		{"from",     required_argument, NULL, 'f'},
		{"to",       required_argument, NULL, 't'},
		{NULL,       no_argument,       NULL, 0}
};

static char* get_filename_extension(char*);
static void show_help(FILE* fp, const char* program_name);
static int is_virtual_pipe_name(const char*);


static void show_help(FILE* fp, const char* program_name) {
	fprintf(fp, "usage: %s [<option>...] [<filter> ...]\n\n", program_name);
	fprintf(fp, "one of -f and -i, one of -t and -o are required.\n"
			"Options:\n"
			"  -f, --from=TYPE      set TYPE as file type of input\n"
			"  -i, --in-file=FILE   read from FILE [default=stdin]\n"
			"  -o, --out-file=FILE  write to FILE. it may break file if\n"
			"                       error occurred [default=stdout]\n"
			"  -t, --to=TYPE        set TYPE as file type of output\n"
			"  -h, --help           show this message and exit\n"
			"\n");
	fprintf(fp, "File types:\n");
	pbmcodec_show_help(fp);
	fprintf(fp, ""
			"If you specify to -i/-o filename with extension which equals to \n"
			"one of supported file types, you can omit -f/-t.\n"
			"\n");
	fprintf(fp, "Filters:\n");
	pbmfilter_show_help(fp);
}

int main(int argc, char** argv) {
	char* infile_name = NULL;
	char* outfile_name = NULL;
	pbmcodec_reader_fn reader = NULL;
	pbmcodec_writer_fn writer = NULL;

	int opt;
	int longindex;
	while ((opt = getopt_long(argc, argv, "hi:o:f:t:", long_options, &longindex)) != -1) {
//		printf("%d %s\n", longindex, long_options[longindex].name);
		switch (opt) {
			case 'i':
				infile_name = optarg;
				break;
			case 'o':
				outfile_name = optarg;
				break;
			case 'f':
				reader = pbmcodec_get_reader(optarg);
				if (!reader) {
					LOG(error, "reader not found: '%s'", optarg);
				}
				break;
			case 't':
				writer = pbmcodec_get_writer(optarg);
				if (!writer) {
					LOG(error, "writer not found: '%s'", optarg);
				}
				break;
			case 'h':
				show_help(stdout, argv[0]);
				return 0;
			case '?':
				show_help(stderr, argv[0]);
				return 1;
			default:
				LOG(error, "logic error: %c, %c", opt, optopt);
				abort();
		}
	}

	FILE* infile = stdin;
	FILE* outfile = stdout;
	if (infile_name != NULL) {
		if (!is_virtual_pipe_name(infile_name)) {
			infile = fopen(infile_name, "rb");
			if (!infile) {
				perror(infile_name);
				return 1;
			}
			if (!reader) {
				reader = pbmcodec_get_reader(get_filename_extension(infile_name));
				if (!reader) {
					LOG(warn, "source type prediction failed: '%s'", optarg);
				}
			}
		}
	}
	if (outfile_name != NULL) {
		if (!is_virtual_pipe_name(outfile_name)) {
			outfile = fopen(outfile_name, "wb");
			if (!outfile) {
				perror(outfile_name);
				return 1;
			}
			if (!writer) {
				writer = pbmcodec_get_writer(get_filename_extension(outfile_name));
				if (!writer) {
					LOG(error, "target type prediction failed: '%s'", optarg);
				}
			}
		}
	}

	if (!reader) {
		LOG(error, "you must specify -f <source-type> or -i <filename.ext>");
		return 1;
	}
	if (!writer) {
		LOG(error, "you must specify -t <target-type> or -o <filename.ext>");
		return 1;
	}

	pbm_info a = {-1, -1, NULL};
	pbm_info b = {-1, -1, NULL};
	pbm_info* prev = &a;
	pbm_info* next = &b;

	if (reader(prev, infile)) {
		return 1;
	}
	for (int i = optind; i < argc; i++) {
		size_t arg_count = 0;
		for (char* p = argv[i]; *p != '\0'; p++) {
			if (*p == ':') {
				arg_count++;
			}
		}
		char** filter_args = (char**) malloc(sizeof(char*) * arg_count);
		char** args_p = filter_args;
		*args_p++ = argv[i];
		for (char* p = argv[i]; *p != '\0'; p++) {
			if (*p == ':') {
				*p++ = '\0';
				*args_p++ = p;
			}
		}

		pbmfilter_fn filter_fn = pbmfilter_get_filter(filter_args[0]);
		if (!filter_fn) {
			LOG(error, "filter not found: '%s'", filter_args[0]);
		}

		if (filter_fn(prev, next, (const char**) (filter_args + 1))) {
			return 1;
		}
		pbm_info* tmp = next;
		next = prev;
		prev = tmp;
	}

	if (writer(prev, outfile)) {
		return 1;
	}
}

static char* get_filename_extension(char* str) {
	char* extension = str;
	for (char* p = str; *p != '\0'; p++) {
		if (*p == '.') {
			extension = ++p;
		}
	}
	return extension;
}

static int is_virtual_pipe_name(const char* s) {
	return s[0] == '-' && s[1] == '\0';
}
