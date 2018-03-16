#include <string.h>
#include "parse_input.h"


u_char parse_input(int num_strings , char** strings) {
    u_int i;
    u_char mask = 0;
    
    // Fill the mask with the specified flags
    for (i=0 ; i<num_strings ; i++) {
        if (strcmp(*(strings+i) , "-i") == 0) {
            mask |= I_FLAG;
        }
        else if (strcmp(*(strings+i) , "-l") == 0) {
            mask |= L_FLAG;
        }
        else if (strcmp(*(strings+i) , "-n") == 0) {
            mask |= N_FLAG;
        }
        else if (strcmp(*(strings+i) , "-c") == 0) {
            mask |= C_FLAG;
        }
        else if (strcmp(*(strings+i) , "-w") == 0) {
            mask |= W_FLAG;
        }
        else if (strcmp(*(strings+i) , "-r") == 0) {
            mask |= R_FLAG;
        }
    }
    
    return mask;
}
