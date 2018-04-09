#include <stdio.h>
#include <stdlib.h>
#include "sighandlers.h"
#include "parse_input.h"
#include "matcher.h"
#include <string.h>

void print_usage(FILE* stream);

int main(int argc, char* argv[], char* envp[]) {

    if(argc == 1) {
        print_usage(stdout);
        return 0;
    }

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

    char * file_path = NULL;
    char * pattern = NULL;

    if(get_file_and_pattern(argc, argv, &file_path, &pattern) != 0) {
        printf("Error in getting file and pattern\n");
        exit(1);
    }

    install_main_handler();

    if(R_FLAG_ACTIVATED(mask)) {
        printf("AAAAAAAAAAAAAAAAAAAAAAH\n\n");
        exit(1);
    } else {
        printf("Testing simple matching of '%s' in file '%s')\n", pattern, file_path);
        
        if(grep_matcher(mask, file_path, pattern) != 0) {
            printf("Error in grep_matcher call!\n");
            return 1;
        }
    }

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