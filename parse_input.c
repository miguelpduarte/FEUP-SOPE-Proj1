#include <string.h>
#include "parse_input.h"


u_char parse_input(int num_strings , char* strings[]) {
    u_int i;
    u_char mask = 0;
    
    // Fill the mask with the specified flags
    for (i=0 ; i<num_strings ; i++) {
        if (strncmp(strings[i] , "-i" , 2) == 0) {
            mask |= I_FLAG;
        }
        else if (strncmp(strings[i], "-l" , 2) == 0) {
            mask |= L_FLAG;
        }
        else if (strncmp(strings[i] , "-n" , 2) == 0) {
            mask |= N_FLAG;
        }
        else if (strncmp(strings[i] , "-c" , 2) == 0) {
            mask |= C_FLAG;
        }
        else if (strncmp(strings[i] , "-w" , 2) == 0) {
            mask |= W_FLAG;
        }
        else if (strncmp(strings[i] , "-r" , 2) == 0) {
            mask |= R_FLAG;
        }
    }
    
    return mask;
}
