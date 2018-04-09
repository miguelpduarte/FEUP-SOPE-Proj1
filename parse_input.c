#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "parse_input.h"


u_char parse_input(int num_strings , char* strings[], int* lastFlagIndex) {
    u_int i, j;
    u_char mask = 0;
    *lastFlagIndex = 0;
    
    // Fill the mask with the specified flags
    for (i=1 ; i<num_strings ; i++) {
        if (strncmp(strings[i], "-", 1) == 0) {
            *lastFlagIndex = i;
        
            for (j=1 ; j<strlen(strings[i]) ; j++) {
                switch(strings[i][j]) {
                case 'i':
                    mask |= I_FLAG;
                    break;
                case 'l':
                    mask |= L_FLAG;
                    break;
                case 'n':
                    mask |= N_FLAG;
                    break;
                case 'c':
                    mask |= C_FLAG;
                    break;
                case 'w':
                    mask |= W_FLAG;
                    break;
                case 'r':
                    mask |= R_FLAG;
                    break;  
                default:
                    mask |= ERR_FLAG;
                    return mask;
                }
            }
            
        } else {
            break;
        }
    }
    
    return mask;
}

int is_path_regular_file(const char *file_path) {
    struct stat path_stat;
    
    if (stat(file_path, &path_stat) != 0) {
        return 0;
    }
    
    return S_ISREG(path_stat.st_mode);
}

int is_path_directory(const char* file_path) {
    struct stat path_stat;
    
    if (stat(file_path, &path_stat) != 0) {
        return 0;
    }
    
    return S_ISDIR(path_stat.st_mode);
}
