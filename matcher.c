#include "matcher.h"
#include "parse_input.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"

int grep_matcher(u_char mask, char * file_path, char * pattern) {
	FILE * file = fopen(file_path, "r");
	
	if(file_path == NULL) {
		//Couldn't open file
		perror(file_path);
		return -1;
	}

	ssize_t n_read;
	size_t len = 0;
	char * line_str = NULL;
	char * match = NULL;
	u_int n_matches = 0;

	while((n_read = getline(&line_str, &len, file)) != -1) {
		/*
		//For testing just printing the line and the other vars
		printf("Read line of size %zu\n", n_read);
		printf("Line: %s\nValue of len: %zd\n", line_str, len);
		*/
		//Attempt to match
		if((match = strstr(line_str, pattern)) != NULL) {
			printf("Found match, needle value: " COLOR_RED "%s" COLOR_RESET, match);
			n_matches++;
		}
	}

	free(line_str);

	//Printing output

	printf("There were %d matches!\n", n_matches);

	//Everything went as expected
	return 0;
}
