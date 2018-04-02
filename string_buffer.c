#include "string_buffer.h"
#include <stdlib.h>
#include <string.h>

string_buffer * create_string_buffer() {
    string_buffer * str_buf = malloc(sizeof(*str_buf));
    if(str_buf == NULL) {
	    return NULL;
    }

    //Ensuring initial values
    str_buf->allocated_size = 0;
    str_buf->current_size = 0;
    str_buf->buffer = NULL;
    return str_buf;
}

string_buffer * create_sized_string_buffer(size_t starting_size) {
    string_buffer * str_buf = create_string_buffer();
    if(str_buf == NULL) {
	    return NULL;
    }

    str_buf->buffer = malloc(starting_size * sizeof(*(str_buf->buffer)));
    if(str_buf->buffer == NULL) {
        destroy_string_buffer(&str_buf);
        return NULL;
    }
    
    str_buf->current_size = starting_size;
    str_buf->allocated_size = starting_size;
    return str_buf;
}

ssize_t append_string(string_buffer * str_buf, const char * append_str) {
    if(str_buf == NULL) {
	    return -1;
    }

    if(append_str == NULL || append_str[0] == '\0') {
	    return 0;
    }

    const size_t append_size = strlen(append_str);

    if(str_buf->allocated_size == 0) {
        //String buffer empty, allocate enough size for the passed string
        //Using callloc to null-initialize the buffer
        str_buf->buffer = calloc((append_size + 1), sizeof(*(str_buf->buffer)));
        if((str_buf)->buffer == NULL) {
            free(str_buf);
            str_buf = NULL;
            return -3;
        }
        //Setting starting sizes
        //\0 character
        (str_buf)->current_size = 1;
        (str_buf)->allocated_size = append_size + 1;
    }

    const size_t necessary_size = (str_buf)->current_size + append_size;
    
    if(str_buf->allocated_size < necessary_size) {
        //String buffer not large enough, reallocate with GROWTH_FACTOR * previous capacity size
        do {
            //The loop serves to ensure that when starting with low values the string_buffer grows enough
            str_buf->allocated_size *= GROWTH_FACTOR;
        } while(str_buf->allocated_size < necessary_size);
        
        char * new_buf = realloc(str_buf->buffer, str_buf->allocated_size);
        if(new_buf == NULL) {
            return -4;
        }
        str_buf->buffer = new_buf;
    }

    //Appending the passed string and updating the current size
    strcat(str_buf->buffer, append_str);
    str_buf->current_size += append_size;
    return append_size;
}

void destroy_string_buffer(string_buffer ** str_buf) {
    if(*str_buf == NULL) {
	    return;
    }

    //Since freeing a NULL is only a NOP, no need to check for it
    free((*str_buf)->buffer);
    free(*str_buf);
    
    //To ensure there are no dangling references
    *str_buf = NULL;
}
