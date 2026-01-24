#ifndef INPUT_BUFFER_H
#define INPUT_BUFFER_H

#include "CommandEnums.h"
#include "Common.h"

typedef struct
{
    char* buffer;
    size_t buffer_size;
    size_t input_length;
} InputBuffer;

extern InputBuffer* new_intput_buffer();
extern void free_input_buffer( InputBuffer* input_buffer );
extern void read_input( InputBuffer* input_buffer );

#endif // INPUT_BUFFER_H