#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include "dbg.h"

#define MAX_LINE 128 // max chars in line
#define MAX_FILE 512 // max lines in file
#define MAX_TOKEN 8 // max input tokens
#define LOG_FILE ".logfind"

/* return an array of filenames and tokens */
char **get_logfile(char *filename)
{
	log_info("OPENING FILE '%s' ...", filename);
	FILE *file_ptr = fopen(filename, "r");
	check(file_ptr, "Could not open file.");

	char **file_list = calloc(MAX_FILE, sizeof(char *));
	check_mem(file_list);
	int n = 0;

	do {
		file_list[n] = calloc(MAX_LINE, sizeof(char));
		check_mem(file_list[n]);
		fgets(file_list[n], MAX_LINE-1, file_ptr);
		strtok(file_list[n], "\n");
		log_info("\tToken %d: %s", n, file_list[n]);

		if(feof(file_ptr)) break;
		n++;
	} while(n < MAX_FILE);

	log_info("CLOSING FILE '%s' ...", filename);
	fclose(file_ptr);

	return file_list;

error:
	return NULL;
}

void token_find(FILE *file_ptr, char *filename, char *token)
{
	char buffer[MAX_LINE];

	do {
		fgets(buffer, MAX_LINE-1, file_ptr);
		if(strstr(buffer, token) != NULL)
		{
			log_info("\tFOUND TOKEN '%s' in file '%s'", token, filename);
			break; // done
		}

		if(feof(file_ptr)) {
			log_info("Did not find token '%s' in file '%s'", token, filename);
			break; // did not find
		}
	} while(1);

	rewind(file_ptr); // reset file to beginning after search
}

void search_file(char *filename, char **token_list, int num_tokens)
{
	log_info("OPENING SEARCH FILE '%s' ...", filename);
	FILE *file_ptr = fopen(filename, "r");
	check(file_ptr, "Could not open file.");

	for(int i = 0; i < num_tokens; i++) {
		token_find(file_ptr, filename, token_list[i]);
	}

// fallthrough
error:
	log_info("CLOSING SEARCH FILE '%s' ...", filename);
	if(file_ptr) fclose(file_ptr);
	return;
}

int main(int argc, char *argv[])
{
	check(argc > 1, "Need an argument.");

	// char *filename = "logfind.c";
	char **token_list = malloc(sizeof(char) * MAX_LINE * MAX_TOKEN);
	int n = 0;

	for(n = 0; n < argc - 1; n++) {
		token_list[n] = argv[n+1];
	}

	check(get_logfile(LOG_FILE), "logfile error.");
	search_file("logfind.c", token_list, n);
	search_file("dbg.h", token_list, n);
	search_file("Makefile", token_list, n);

	free(token_list);
	return 0;

error:
	return 1;
}