#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include "psteg.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char *argp_program_version = "psteg 0.1";
const char *argp_program_bug_address = "<proch92@gmail.com>";

/* Program documentation. */
static char doc[] = "psteg - read the README";

/* A description of the arguments we accept. */
static char args_doc[] = "info lsb-extract lsb-create alpha";

/* The options we understand. */
static struct argp_option options[] = {
	{"info", 'i', 0, 0, "Extract general info about image"},
	{"lsbextract", 'e', 0, 0, "Extract data from least significant bytes"},
	{"colororder", 'c', "orderstring", 0, "Specify color byte order for lsbextract and lsbput. Default: \"rgb\""},
	{"numbit", 'n', "nbit", 0, "Specify how many bits have to be used to extract hidden data, Default: 1"},
	{"alpha", 'a', 0, 0, "Extract informations from alpha layer"},
	{0}
};

/* Used by main to communicate with parse_opt. */
struct arguments {
	char functions[3]; //0:info, 1:extraclsb, 2:alpha
	char *filearg;
	char *order;
	char nbit;
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;

	switch(key) {
		case 'i':
			arguments->functions[0] = 1;
			break;
		case 'e':
			arguments->functions[1] = 1;
			break;
		case 'c':
			arguments->order = arg;
			break;
		case 'n':
			arguments->nbit = atoi(arg);
			break;
		case 'a':
			arguments->functions[2] = 1;
			break;

		case ARGP_KEY_ARG:
			if(state->arg_num == 0)
				arguments->filearg = arg;
			else
				return ARGP_ERR_UNKNOWN;
			break;

		case ARGP_KEY_END:
			break;

		default:
			return ARGP_ERR_UNKNOWN;
		}
	return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};

int main (int argc, char **argv) {
	struct arguments arguments;

	/* Default values. */
	arguments.functions[0] = 0;
	arguments.functions[1] = 0;
	arguments.functions[2] = 0;
	arguments.nbit = 1;
	arguments.filearg = "";
	arguments.order = "rgb";

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if(arguments.functions[0] || arguments.functions[0]+arguments.functions[1]+arguments.functions[2] == 0)
		info(arguments.filearg);
	if(arguments.functions[1])
		lsbextract(arguments.filearg, arguments.order, arguments.nbit);
	if(arguments.functions[2])
		alpha(arguments.filearg);

	return 0;
}