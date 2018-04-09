#include <string.h>
#include "parse_input.h"


u_char parse_input(int num_strings , char* strings[]) {
    u_int i, j;
    u_char mask = 0;
    
    // Fill the mask with the specified flags
    for (i=1 ; i<num_strings ; i++) {
        if (strncmp(strings[i], "-", 1) == 0) {
        
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
            
        }
    }
    
    return mask;
}

int get_file_and_pattern(int argc, char * argv[], char ** file_path, char ** pattern) {
    u_int i = 0;
    *file_path = NULL;
    *pattern = NULL;

    for(i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-", 1) != 0) {
            if(*pattern == NULL) {
                *pattern = argv[i];
            } else {
                *file_path = argv[i];
                break;
            }
        }
    }

    if(*file_path == NULL || *pattern == NULL) {
        return 1;
    }

    return 0;
}