#include "matcher.h"
#include "parse_input.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "string_buffer.h"

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_BLUE    "\x1b[34m"


bool is_pattern_in_line(u_char mask, const char * line, const char * pattern) {
	//Using function pointer to simplify code
	char * (*search_func) (const char *, const char *);

	if(I_FLAG_ACTIVATED(mask)) {
	    //Case independent search
	    search_func = strcasestr;
	} else {
	    //Case dependent search
	    search_func = strstr;
	}

	if(W_FLAG_ACTIVATED(mask)) {
	    //Match must not have any non whitespace character before or after itself
	    char * match = (*search_func)(line, pattern);

		//Because of the ability for there to be several matches we must continue to test the line
		while(match != NULL) {
			if((match == line || *(match - 1) == ' ') && (*(match + strlen(pattern)) == '\0' || *(match + strlen(pattern)) == ' ' || *(match + strlen(pattern)) == '\n')) {
				return true;
			}
			
			//Updating match
			match = (*search_func)(match + 1, pattern);
		}

		return false;
	} else {
	    return (*search_func)(line, pattern) != NULL;
	}

	
}

int grep_matcher(u_char mask, const char * file_path, const char * pattern) {
	if(file_path == NULL || pattern == NULL) {
		fprintf(stderr, "Pattern or file was null in grep matcher call!\n");
		return 1;
	}

	FILE * file = fopen(file_path, "r");
	
	if(file == NULL) {
	    //Couldn't open file
	    perror(file_path);
	    return -1;
	}

	string_buffer * str_buf = create_string_buffer();
	if(str_buf == NULL) {
	    printf("Error creating string buffer!\n");
	    fclose(file);
	    return -2;
	}

	ssize_t n_read;
	size_t len = 0;
	char * line_str = NULL;
	u_int n_matches = 0;
	//The buffer to where output is written before being appended
	char * output_buf = NULL;
	

	while((n_read = getline(&line_str, &len, file)) != -1) {
		//Attempt to match
		if(is_pattern_in_line(mask, line_str, pattern)) {
			//Match ocurrred according to current matching flags (i or w)
			
			//Displaying output depending on flags
			if(L_FLAG_ACTIVATED(mask)) {
			    //This flag only needs to know if there is 1 occurrence
			    n_matches++;
			    break;
			}

			if(C_FLAG_ACTIVATED(mask)) {
			    //This flag only needs to count the number of line ocurrences per file
			    n_matches++;
			} else {
			    //Otherwise we want full output
				//Note: No need to append '\n' because the original line already has one at the end
				if(R_FLAG_ACTIVATED(mask)) {
			    	if(asprintf(&output_buf, COLOR_BLUE "%s:" COLOR_RESET "%s", file_path, line_str) == -1) {
						fprintf(stderr, "Error in asprintf, inside grep_matcher!\n");
						return -3;
					}
				} else {
					if(asprintf(&output_buf, "%s", line_str) == -1) {
						fprintf(stderr, "Error in asprintf, inside grep_matcher!\n");
						return -4;
					}
				}
				append_string(str_buf, output_buf);
				free(output_buf);
			}

		}
	}

	free(line_str);

	//Printing output

	if(C_FLAG_ACTIVATED(mask)) {
		if(R_FLAG_ACTIVATED(mask)) {
			if(asprintf(&output_buf, COLOR_BLUE "%s:" COLOR_RESET "%d\n", file_path, n_matches) == -1) {
				fprintf(stderr, "Error in asprintf, inside grep_matcher!\n");
				return -5;
			}
		} else {
			if(asprintf(&output_buf, "%d\n", n_matches) == -1) {
				fprintf(stderr, "Error in asprintf, inside grep_matcher!\n");
				return -6;
			}
		}
	    
		append_string(str_buf, output_buf);
	    free(output_buf);
	} else if(L_FLAG_ACTIVATED(mask) && n_matches > 0) {
	    if(asprintf(&output_buf, "%s\n", file_path) == -1) {
			fprintf(stderr, "Error in asprintf, inside grep_matcher!\n");
			return -7;
		}
	    append_string(str_buf, output_buf);
	    free(output_buf);
	}

	//Everything went as expected
	//Writing output if there is any to write
	if(str_buf->current_size > 0) {
	    size_t n_written = write(STDOUT_FILENO, str_buf->buffer, str_buf->current_size * sizeof(*(str_buf->buffer)));
	    if(n_written == 0) {
			fprintf(stderr, "Problem in write, inside grep_matcher!\n");
			return -8;
	    }
	}
	//"Cleaning up"
	destroy_string_buffer(&str_buf);
	fclose(file);
	return 0;
}

int grep_matcher_stdin(u_char mask, const char * pattern) {
	if(pattern == NULL) {
		fprintf(stderr, "Pattern was null in grep matcher from stdin call!\n");
		return 1;
	}

	string_buffer * str_buf = create_string_buffer();
	if(str_buf == NULL) {
	    printf("Error creating string buffer!\n");
	    return -2;
	}

	ssize_t n_read;
	size_t len = 0;
	char * line_str = NULL;
	u_int n_matches = 0;

	while((n_read = getline(&line_str, &len, stdin)) != -1) {
		//Attempt to match
		if(is_pattern_in_line(mask, line_str, pattern)) {
			//Match ocurrred according to current matching flags (i or w)
			
			//Displaying output depending on flags
			if(L_FLAG_ACTIVATED(mask)) {
			    //This flag only needs to know if there is 1 occurrence
			    n_matches++;
			    break;
			}

			if(C_FLAG_ACTIVATED(mask)) {
			    //This flag only needs to count the number of line ocurrences per file
			    n_matches++;
			} else {
			    //Otherwise we want full output (in this case only the line)
				//Note: No need to append '\n' because the original line already has one at the end
				append_string(str_buf, line_str);
			}

		}
	}

	free(line_str);

	//Printing output

	if(C_FLAG_ACTIVATED(mask)) {
		char * output_buf = NULL;
		if(asprintf(&output_buf, "%d\n", n_matches) == -1) {
			fprintf(stderr, "Error in asprintf, inside grep_matcher!\n");
			return -6;
		}
		append_string(str_buf, output_buf);
	    free(output_buf);
	} else if(L_FLAG_ACTIVATED(mask) && n_matches > 0) {
	    append_string(str_buf, COLOR_BLUE "(standard input)" COLOR_RESET);
	}

	//Everything went as expected
	//Writing output if there is any to write
	if(str_buf->current_size > 0) {
	    size_t n_written = write(STDOUT_FILENO, str_buf->buffer, str_buf->current_size * sizeof(*(str_buf->buffer)));
	    if(n_written == 0) {
			fprintf(stderr, "Problem in write, inside grep_matcher!\n");
			return -8;
	    }
	}
	//"Cleaning up"
	destroy_string_buffer(&str_buf);
	return 0;
}
