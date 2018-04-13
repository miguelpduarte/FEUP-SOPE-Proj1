#include "recursive_grep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include "matcher.h"
#include "logfile.h"

void recursive_grep(unsigned char mask, const char* path, const char* pattern) {

    DIR* dir_ptr;
    struct dirent* dirent_ptr;
    struct stat entry_info;
    char entry_name[MAX_ENTRY_NAME_SIZE];
    u_int process_counter = 0;
    
    // Open the directory
    if ( (dir_ptr = opendir(path)) == NULL) {
        perror(path);
        exit(ERROR_OPENING_DIRECTORY);
    }
    
    // Parse all the entries in the directory
    while ( (dirent_ptr = readdir(dir_ptr)) != NULL) {
        // Update the entry name
        sprintf(entry_name, "%s/%s", path, dirent_ptr->d_name);
        
        // Obtain the entry informations
        if (lstat(entry_name , &entry_info) == -1) {
            perror("Error in lstat() call");
            exit(ERROR_LSTAT_CALL);
        }
        
        // Check which type of entry it is
        if (S_ISREG(entry_info.st_mode)) {
            //Running text matcher in another process
            int fork_result = fork();
            if(fork_result < 0) {
                exit(ERROR_FORK);
            }

            //Parent should just continue exploring directory as usual
            if(fork_result == 0) {
                //Child
                char * buffer = NULL;
                asprintf(&buffer, "Created process with pid %.8d (child-rgrep-regfile)", getpid());
                writeinLog(buffer);
                free(buffer);
                exit(grep_matcher(mask, entry_name, pattern));
            }
            else {
                //Parent
                process_counter++;
            }
        }
        else if (S_ISDIR(entry_info.st_mode)) {            
            // Call 'ls' in the directory, if it isn't . (current directory) and if it isn't .. (previous directory)
            if (strncmp(dirent_ptr->d_name , "." , 1) != 0 &&
                strncmp(dirent_ptr->d_name , "..", 2) != 0) {
                
                //Exploring subdirectories in another process
                int fork_result = fork();
                if(fork_result < 0) {
                    exit(ERROR_FORK);
                }

                //Parent should just continue exploring directory as usual
                if(fork_result == 0) {
                    //Child
                    recursive_grep(mask, entry_name, pattern);
                    char * buffer = NULL;
                    asprintf(&buffer, "Created process with pid %.8d (child-rgrep-dir)", getpid());
                    writeinLog(buffer);
                    free(buffer);
                    exit(0);
                }
                else {
                    //Parent
                    process_counter++;
                }
            }
        }
    }
    
    closedir(dir_ptr);
    
    // Wait for all the created processes to return
    int return_value, wait_return;
    while(process_counter > 0) {
        wait_return = wait(&return_value);
        
        if (wait_return == -1) {
            exit(ERROR_WAIT);
        }
        else {
            if (return_value != 0) {
                exit(return_value);
            }
            else {
                process_counter--;
            }
        } 
    }
    
    exit(0);
}
