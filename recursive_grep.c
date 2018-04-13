#include "recursive_grep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "matcher.h"


void recursive_grep(unsigned char mask, const char* path, const char* pattern) {

    DIR* dir_ptr;
    struct dirent* dirent_ptr;
    struct stat entry_info;
    char entry_name[MAX_ENTRY_NAME_SIZE];
    //char* type_of_entry;
    
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
            //type_of_entry = "regular";
            //Running text matcher in another process
            int fork_result = fork();
            if(fork_result < 0) {
                exit(ERROR_FORK);
            }

            //Parent should just continue exploring directory as usual
            if(fork_result == 0) {
                //Child
                exit(grep_matcher(mask, entry_name, pattern));
                exit(0);
            }
        }
        else if (S_ISDIR(entry_info.st_mode)) {
            //type_of_entry = "directory";
            
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
                    exit(0);
                }
            }
        }
        /*
        else {
            type_of_entry = "other";
        }
        */
        
        // Print the entry info.
        //fprintf(stdout , "%-80s - %s\n" , entry_name , type_of_entry);
    }
    
    closedir(dir_ptr);
    exit(0);
}
