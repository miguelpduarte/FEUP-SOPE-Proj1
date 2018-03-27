#ifndef _STRING_BUFFER_H_
#define _STRING_BUFFER_H_

#include <stdlib.h>

typedef struct {
	//The currently allocated size of the string
	size_t allocated_size;
	//The currently used size of the string
	size_t current_size;
	//The data buffer
	char * buffer;
} string_buffer;

///Implementation note:
/*
 * The string_buffer current_size, is at least 1, to consider the null terminator.
 * This also makes it so that when appending strings, we need only increment by the size of the given string,
 * excluding its own null terminator, as only 1 is ever necessary.
 */

#define GROWTH_FACTOR 2

/**
 * Creates an empty string_buffer
 */
string_buffer * create_string_buffer();

/**
 * Creates a string_buffer with the specified starting size
 */
string_buffer * create_sized_string_buffer(size_t starting_size);

/**
 * Appends the given string to the given string_buffer.
 * Returns the number of characters appended or negative in case of error.
 * If append_str is NULL or empty, nothing is done
 * string_buffer * should be deallocated using the 'destroy_string_buffer' function and allocated using the 'create_string_buffer' functions
 */
ssize_t append_string(string_buffer * str_buf, const char * append_str);

/**
 * Deallocates a string_buffer fully, and also sets the calling variable to NULL to prevent dangling references.
 * (Hence the ** instead of only *)
 */
void destroy_string_buffer(string_buffer ** str_buf);

#endif /* _STRING_BUFFER_H_ */
