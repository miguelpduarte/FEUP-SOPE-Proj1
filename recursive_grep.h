#ifndef _RECURSIVE_LS_H_
#define _RECURSIVE_LS_H_

#define MAX_ENTRY_NAME_SIZE 256
#define INVALID_PARAMETERS 1
#define ERROR_OPENING_DIRECTORY 2
#define ERROR_LSTAT_CALL 3
#define ERROR_FORK 4


/*
 *  Performs a recursive 'ls' in the folder with 'path' passed in as parameter.
 *  Return: void.
 */
void recursive_grep(unsigned char mask, const char* path, const char* pattern);


#endif  /* _RECURSIVE_LS_H_ */
