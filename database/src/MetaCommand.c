#include "MetaCommand.h"

#include <stdlib.h>

MetaCommandResult execute_meta_command( InputBuffer* input_buffer, Table* table )
{
    if( strcmp( input_buffer->buffer, ".exit" ) == 0 )
    {
        free_input_buffer( input_buffer );
        db_close( table );
        exit( EXIT_SUCCESS );
    }
    else if( strcmp( input_buffer->buffer, ".write" ) == 0 )
    {
        log_info( "Attempting to write database..." );
        pager_flush( table->pager, table->num_rows / ROWS_PER_P AGE, PAGE_SIZE );
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }

    return META_COMMAND_SUCCESS;
}