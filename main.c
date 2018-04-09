#include <stdio.h>
#include <stdlib.h>
#include "sighandlers.h"
#include "parse_input.h"
#include "matcher.h"
#include <string.h>

#define INVALID_FLAGS   1
#define MISSING_PATTERN 2
#define MISSING_PATH    3
#define INVALID_PATH    4

void print_usage(FILE* stream);

int main(int argc, char* argv[], char* envp[]) {

    if(argc == 1) {
        print_usage(stdout);
        return 0;
    }

    // Parsing Mask Testing
    int lastFlagIndex;
    u_char mask = parse_input(argc, argv, &lastFlagIndex);
    int pattern_index = lastFlagIndex + 1;
    int file_path_index = lastFlagIndex + 2;
    
    // Invalid Flag was found by input parser
    if (ERR_FLAG_ACTIVATED(mask)) {
        fprintf(stderr, "Invalid option.\n\n");
        print_usage(stderr);
        return INVALID_FLAGS;
    }
    
    // Check if user entered a pattern 
    if (pattern_index >= argc) {
        fprintf(stderr, "Pattern missing.\n\n");
        print_usage(stderr);
        return MISSING_PATTERN;
    }

    // Install the main process interrupt handlers
    install_main_handler();
    
    
    if (R_FLAG_ACTIVATED(mask)) {
        // When the -r flag is specified, can not read from stdin, a file/directory must be specified.
        if (file_path_index >= argc) {
            fprintf(stderr, "Please enter a directory path when specifying the '-r' flag.\n\n");
            print_usage(stderr);
            return MISSING_PATH;
        }
    
        if (is_path_regular_file(argv[file_path_index])) {
            // Unset -r flag
            mask &= ~R_FLAG;
            // TODO RECURSIVE GREP
        }
        else if (is_path_directory(argv[file_path_index])) {
            // TODO RECURSIVE GREP
        }
        else {
            fprintf(stderr, "Path does not correspond to a file/directory.\n\n");
            return INVALID_PATH;
        }
        
        
    }
    else {
        /* Check if the user entered a file path. If not, read from stdin */
        if (file_path_index >= argc) {
            return grep_matcher_stdin(mask, argv[pattern_index]);
        }
        
        // Verify if file is a regular file or a directory
        else if (!is_path_regular_file(argv[file_path_index])) {    
            fprintf(stderr, "Path must correspond to a regular file.\n\n");
            return INVALID_PATH;
        }
        else {
            return grep_matcher(mask, argv[file_path_index], argv[pattern_index]);
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
