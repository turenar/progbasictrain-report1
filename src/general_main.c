#include "config.inc.h"

#include <stdlib.h>
#include <getopt.h>
#include "pbmutil.h"
#include "codec/codecs.h"
#include "logger.h"


typedef struct {
	char* infile_name;
	char* outfile_name;
	pbmcodec_reader_fn reader;
	pbmcodec_writer_fn writer;
	filter_info* filters;
} app_opts;

static struct option long_options[] = {
		{"help",     no_argument,       NULL, 'h'},
		{"in-file",  required_argument, NULL, 'i'},
		{"out-file", required_argument, NULL, 'o'},
		{"from",     required_argument, NULL, 'f'},
		{"to",       required_argument, NULL, 't'},
		{NULL,       no_argument,       NULL, 0}
};

static char* get_filename_extension(char*);
static int is_virtual_pipe_name(const char*);
static void parse_opts(int argc, char** argv, app_opts* ret);
static void parse_filters(int argc, char** argv, app_opts* ret);


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
	app_opts opts = {NULL, NULL, NULL, NULL, NULL};
	int result = 1;

	parse_opts(argc, argv, &opts);
	parse_filters(argc, argv, &opts);
	FILE* infile = stdin;
	FILE* outfile = stdout;
	if (opts.infile_name != NULL) {
		if (!is_virtual_pipe_name(opts.infile_name)) {
			infile = fopen(opts.infile_name, "rb");
			if (!infile) {
				perror(opts.infile_name);
				return 1;
			}
			if (!opts.reader) {
				opts.reader = pbmcodec_get_reader(get_filename_extension(opts.infile_name));
				if (!opts.reader) {
					LOG(warn, "source type prediction failed: '%s'", optarg);
				}
			}
		}
	}
	if (opts.outfile_name != NULL) {
		if (!is_virtual_pipe_name(opts.outfile_name)) {
			outfile = fopen(opts.outfile_name, "wb");
			if (!outfile) {
				perror(opts.outfile_name);
				return 1;
			}
			if (!opts.writer) {
				opts.writer = pbmcodec_get_writer(get_filename_extension(opts.outfile_name));
				if (!opts.writer) {
					LOG(error, "target type prediction failed: '%s'", optarg);
				}
			}
		}
	}

	if (!opts.reader) {
		LOG(error, "you must specify -f <source-type> or -i <filename.ext>");
		return 1;
	}
	if (!opts.writer) {
		LOG(error, "you must specify -t <target-type> or -o <filename.ext>");
		return 1;
	}

	pbm_info a;
	pbm_info b;
	pbm_info* prev = &a;
	pbm_info* next = &b;
	pbm_init(&a);
	pbm_init(&b);

	if (opts.reader(prev, infile)) {
		goto pbm_error;
	}

	for (filter_info* p = opts.filters; p->args != NULL; p++) {
		if (p->fn(prev, next, p->args + 1)) {
			goto pbm_error;
		}
		pbm_info* tmp = prev;
		prev = next;
		next = tmp;
	}

	if (opts.writer(prev, outfile)) {
		goto pbm_error;
	}
	result = 0;

pbm_error:
	for (filter_info* p = opts.filters; p->args != NULL; p++) {
		pbm_free_filter_info(p);
	}
	free(opts.filters);
	pbm_free(&a);
	pbm_free(&b);
	return result;
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

static void parse_opts(int argc, char** argv, app_opts* ret) {
	int opt;
	int longindex;
	while ((opt = getopt_long(argc, argv, "hi:o:f:t:", long_options, &longindex)) != -1) {
//		printf("%d %s\n", longindex, long_options[longindex].name);
		switch (opt) {
			case 'i':
				ret->infile_name = optarg;
				break;
			case 'o':
				ret->outfile_name = optarg;
				break;
			case 'f':
				ret->reader = pbmcodec_get_reader(optarg);
				if (!ret->reader) {
					LOG(error, "reader not found: '%s'", optarg);
				}
				break;
			case 't':
				ret->writer = pbmcodec_get_writer(optarg);
				if (!ret->writer) {
					LOG(error, "writer not found: '%s'", optarg);
				}
				break;
			case 'h':
				show_help(stdout, argv[0]);
				exit(0);
			case '?':
				show_help(stderr, argv[0]);
				exit(1);
			default:
				LOG(error, "logic error: %c, %c", opt, optopt);
				abort();
		}
	}
}

static void parse_filters(int argc, char** argv, app_opts* ret) {
	int filter_count = argc - optind;
	filter_info* filters = (filter_info*) malloc(sizeof(filter_info) * (size_t) (filter_count + 1));
	filter_info* p = filters;
	for (int i = 0; i < filter_count; i++) {
		pbm_error_t error = pbm_parse_filter_str(argv[optind + i], p++);
		if (error) {
			exit(1);
		}
	}
	// last element
	p->args = NULL;
	p->fn = NULL;
	ret->filters = filters;
}
