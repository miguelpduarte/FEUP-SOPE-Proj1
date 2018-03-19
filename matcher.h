#ifndef _MATCHER_H_
#define _MATCHER_H_

/* For u_char type */
#include "parse_input.h"

/**
 *	Matches the given pattern against the given file, while considering the flags in the passed mask
 *	Returns 0 in case of success or non zero in case of failure. Writes output to STDOUT
 */
int grep_matcher(u_char mask, char * file_path, char * pattern);

#endif /* _MATCHER_H_ */
