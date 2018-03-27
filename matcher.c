#include "matcher.h"
#include "parse_input.h"
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_BLUE    "\x1b[34m"


bool is_pattern_in_line(u_char mask, const char * line, const char * pattern) {
	if(W_FLAG_ACTIVATED(mask)) {
		//TODO
		return false;
	}

	if(I_FLAG_ACTIVATED(mask)) {
		//Case independent search
		return strcasestr(line, pattern) != NULL;
	} else {
		//Case dependent search
		return strstr(line, pattern) != NULL;
	}
}

int grep_matcher(u_char mask, const char * file_path, const char * pattern) {
	FILE * file = fopen(file_path, "r");
	
	if(file == NULL) {
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
		//Attempt to match
		if(is_pattern_in_line(mask, line_str, pattern)) {
			//Match ocurrred according to current matching flags (i or w)
			
			//Displaying output depending on flags
			if(C_FLAG_ACTIVATED(mask)) {
				n_matches++;
			} else if(L_FLAG_ACTIVATED(mask)) {
				n_matches++;
				break;
			}

			printf(COLOR_BLUE "%s:" COLOR_RESET "%s", file_path, line_str);
			////printf("Found match, needle value: " COLOR_RED "%s" COLOR_RESET, match);
			n_matches++;
		}
	}

	free(line_str);

	//Printing output

	if(C_FLAG_ACTIVATED(mask)) {
		printf("%d\n", n_matches);
	} else if(L_FLAG_ACTIVATED(mask) && n_matches > 0) {
		printf("%s\n", file_path);
	}

	//Everything went as expected
	fclose(file);
	return 0;
}


