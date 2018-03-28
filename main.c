#include <stdio.h>
#include "parse_input.h"
#include "matcher.h"
#include "string_buffer.h"
#include <string.h>

int main(int argc , char* argv[] , char* envp[]) {

    /* Parsing Mask Testing */
    u_char mask = parse_input(argc , argv);
    
    printf("\nMask: %X\n\n" , mask);
    
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
