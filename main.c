#include <stdio.h>
#include "parse_input.h"
#include "matcher.h"
#include "string_buffer.h"
#include <string.h>

void print_usage(FILE* stream);

int main(int argc , char* argv[] , char* envp[]) {

    /* Parsing Mask Testing */
    u_char mask = parse_input(argc , argv);
    
    if (ERR_FLAG_ACTIVATED(mask)) {
        fprintf(stderr, "Invalid option.\n\n");
        print_usage(stderr);
        return 1;
    }
    
    printf("-i flag is active: %d\n" , I_FLAG_ACTIVATED(mask));
    printf("-l flag is active: %d\n" , L_FLAG_ACTIVATED(mask));
    printf("-n flag is active: %d\n" , N_FLAG_ACTIVATED(mask));
    printf("-c flag is active: %d\n" , C_FLAG_ACTIVATED(mask));
    printf("-w flag is active: %d\n" , W_FLAG_ACTIVATED(mask));
    printf("-r flag is active: %d\n\n" , R_FLAG_ACTIVATED(mask));

    printf("Testing simple matching of 'test' in file 'test_files/test.txt')\n");
        
    if(grep_matcher(mask, "test_files/test.txt", "test") != 0) {
	printf("Error in grep_matcher call!\n");
	return 1;
    }
    
    /*
    printf("Testing string_buffer usage\n");

    string_buffer * str_buf = create_string_buffer();
    int i;
    for(i = 0; i < 11; ++i) {
	append_string(str_buf, "a");
	if(i == 5 || i == 8) {
	    append_string(str_buf, "wow");
	}
	printf("current_size:%zu  allocated_size:%zu text:%s\n", str_buf->current_size, str_buf->allocated_size, str_buf->buffer);
    }

    destroy_string_buffer(&str_buf);
    */

    return 0;
}

void print_usage(FILE* stream) {
    fprintf(stream, "usage: simgrep [options] pattern [file/dir]\n");
    fprintf(stream, "\noptions:");
    fprintf(stream, "\n-i    ignore case sensitivity in pattern to find");
    fprintf(stream, "\n-l    display only the files names where the pattern is present");
    fprintf(stream, "\n-n    indicate line numbering within the file where the pattern is present");
    fprintf(stream, "\n-c    indicate in how many lines the pattern is present");
    fprintf(stream, "\n-w    select only lines containing matches that form whole  words");
    fprintf(stream, "\n-r    search the pattern recursively in all the files under the given directory\n\n");
}
