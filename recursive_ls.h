#ifndef _RECURSIVE_LS_H_
#define _RECURSIVE_LS_H_

#define MAX_ENTRY_NAME_SIZE 256
#define INVALID_PARAMETERS 1
#define ERROR_OPENING_DIRECTORY 2
#define ERROR_LSTAT_CALL 3


/*
 *  Performs a recursive 'ls' in the folder with 'path' passed in as parameter.
 *  Return: void.
 */
void recursive_ls(char path[]);


#endif  /* _RECURSIVE_LS_H_ */
