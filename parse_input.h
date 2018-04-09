#ifndef _PARSE_INPUT_H_
#define _PARSE_INPUT_H_

/* Typedefs */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

/* Bit Mask */
#define BIT(n)  (0x01<<(n))
#define I_FLAG  BIT(0)
#define L_FLAG  BIT(1)
#define N_FLAG  BIT(2)
#define C_FLAG  BIT(3)
#define W_FLAG  BIT(4)
#define R_FLAG  BIT(5)
#define ERR_FLAG  BIT(7)

/* Flag-Verification Macros */
#define I_FLAG_ACTIVATED(mask)  ((mask) & (I_FLAG) ? (1) : (0))
#define L_FLAG_ACTIVATED(mask)  ((mask) & (L_FLAG) ? (1) : (0))
#define N_FLAG_ACTIVATED(mask)  ((mask) & (N_FLAG) ? (1) : (0))
#define C_FLAG_ACTIVATED(mask)  ((mask) & (C_FLAG) ? (1) : (0))
#define W_FLAG_ACTIVATED(mask)  ((mask) & (W_FLAG) ? (1) : (0))
#define R_FLAG_ACTIVATED(mask)  ((mask) & (R_FLAG) ? (1) : (0))
#define ERR_FLAG_ACTIVATED(mask)  ((mask) & (ERR_FLAG) ? (1) : (0))

/*
 *  Checks for flags in the 'strings' array and fills 'mask' with the flags that were set.
 *  Return: A mask with the flags that were set.
 */
u_char parse_input(int num_strings , char** strings, int* lastFlagIndex);

/*
 *  Checks if a file is a regular file or not.
 *  Return: True if is a regular file, false otherwise
 */
int is_path_regular_file(const char* file_path);

/*
 *  Checks if a file is a directory or not.
 *  Return: True if is a directory, false otherwise
 */
int is_path_directory(const char* file_path);

#endif  /* _PARSE_INPUT_H_ */
