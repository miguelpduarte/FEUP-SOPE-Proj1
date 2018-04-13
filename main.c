#include <stdio.h>
#include <stdlib.h>
#include "sighandlers.h"
#include "parse_input.h"
#include "matcher.h"
#include "logfile.h"
#include "recursive_grep.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE     32

#define INVALID_FLAGS   1
#define MISSING_PATTERN 2
#define MISSING_PATH    3
#define INVALID_PATH    4
#define FORK_FAILURE    5

void print_usage(FILE* stream);
int recursive_explorer(u_char mask, const char* file_path, const char* pattern);

int main(int argc, char* argv[]) {

    if(argc == 1) {
        print_usage(stdout);
        return 0;
    }

    initLog();

    char command[7 + argc * BUFFER_SIZE + (argc-1) + 1];
    command[0] = '\0';
    strcat(command, "COMANDO");
    int i;
    for(i = 0; i < argc; ++i) {
        strncat(command, " ", 1);
        strncat(command, argv[i], BUFFER_SIZE);
    }

    writeinLog(command);

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
            return grep_matcher(mask, argv[file_path_index], argv[pattern_index]);
        } else if (is_path_directory(argv[file_path_index])) {
            return recursive_explorer(mask, argv[file_path_index], argv[pattern_index]);
        } else {
            fprintf(stderr, "Path does not correspond to a file/directory.\n\n");
            return INVALID_PATH;
        }
        
        
    } else {
        // Check if the user entered a file path. If not, read from stdin
        if (file_path_index >= argc) {
            return grep_matcher_stdin(mask, argv[pattern_index]);
        } else if (!is_path_regular_file(argv[file_path_index])) {
            // Verify if file is a regular file or a directory
            fprintf(stderr, "Path must correspond to a regular file.\n\n");
            return INVALID_PATH;
        } else {
            return grep_matcher(mask, argv[file_path_index], argv[pattern_index]);
        }
        
    }

    finishLog();
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

int recursive_explorer(u_char mask, const char* initial_path, const char* pattern) {
    //Removing trailing '/'
    char * initial_path_no_dash = strdup(initial_path);
    if(initial_path_no_dash[strlen(initial_path_no_dash) - 1] == '/') {
        initial_path_no_dash[strlen(initial_path_no_dash) - 1] = '\0';
    }

    int fork_result = fork();
    if(fork_result < 0) {
        fprintf(stderr, "Fork failed!\n");
        return FORK_FAILURE;
    }

    if(fork_result > 0) {
        char * buff = NULL;
        asprintf(&buff, "Created process with pid %.8d (parent)", fork_result);
        writeinLog(buff);
        free(buff);

        sleep(1);

        //Parent
        siginfo_t info;
        while (waitid(P_ALL, -1, &info, WEXITED) != -1) {
            char * buffer = NULL;
            asprintf(&buffer, "Process with pid %.8d terminated with code %d", info.si_pid, info.si_status);
            writeinLog(buffer);
            free(buffer);
        }
        return 0;
    } else {
        //Child
        char * buffer = NULL;
        asprintf(&buffer, "Created process with pid %.8d (child)", getpid());
        writeinLog(buffer);
        free(buffer);
        install_child_handler();
        recursive_grep(mask, initial_path_no_dash, pattern);
        exit(0);
    }
}
