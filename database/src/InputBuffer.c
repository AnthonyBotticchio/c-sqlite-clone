#include "InputBuffer.h"

#include <log.h>
#include <stdlib.h>

#define BUFFER_CAP 256

InputBuffer* new_intput_buffer()
{
    InputBuffer* input_buffer  = malloc( sizeof( InputBuffer ) ); // Allocate InputBuffer sized space in memory
    input_buffer->buffer       = NULL;
    input_buffer->buffer_size  = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void free_input_buffer( InputBuffer* input_buffer )
{
    if( input_buffer )
    {
        free( input_buffer->buffer );
        input_buffer->buffer = NULL;
        free( input_buffer );
    }
}

void read_input( InputBuffer* input_buffer )
{
    if( input_buffer )
    {
        // Allocate buffer if not already allocated
        if( input_buffer->buffer == NULL )
        {
            input_buffer->buffer_size = BUFFER_CAP; // initial size (NOB_DA_INIT_CAP = 256)
            input_buffer->buffer      = malloc( input_buffer->buffer_size );
            if( input_buffer->buffer == NULL )
            {
                log_fatal( "Memory allocation failed." );
                exit( EXIT_FAILURE );
            }
        }

        // Read stdin from user. Provide a template prompt
        printf( PROMPT );

        if( fgets( input_buffer->buffer, input_buffer->buffer_size, stdin ) == NULL )
        {
            log_fatal( "Error reading input. Exiting..." );
            exit( EXIT_FAILURE );
        }

        // Calculate input length
        input_buffer->input_length = strlen( input_buffer->buffer );

        // Remove trailing newline if present
        if( input_buffer->input_length > 0 && input_buffer->buffer[input_buffer->input_length - 1] == '\n' )
        {
            input_buffer->buffer[input_buffer->input_length - 1] = '\0'; // All strings in c must end with the terminating character.
            input_buffer->input_length--;
        }

        log_trace( "Recorded input: '%s%s'", PROMPT, input_buffer->buffer );
    }
    else
    {
        log_fatal( "Tried to read input when input_buffer is NULL." );
        exit( EXIT_FAILURE );
    }
}