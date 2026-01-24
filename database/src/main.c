#define LOGGING_PATH_TRACE "log_trace.txt"
#define LOGGING_PATH_INFO "log_info.txt"
#define LOGGING_PATH_WARNING "log_warning.txt"
#define LOGGING_PATH_FATAL "log_fatal.txt"

#include "Common.h"
#include "MetaCommand.h"
#include "Row.h"
#include "Table.h"

#include <log.h>
#include <stdlib.h>

PrepareResult prepare_insert( InputBuffer* input_buffer, Statement* statement )
{
    statement->type = STATEMENT_INSERT;

    char* keyword = strtok( input_buffer->buffer, " " ); // uses strtok to split a string into tokens based on spaces here.
                                                         // It also remembers its position in the string for subsequent calls.
    char* id_string = strtok( NULL, " " );
    char* username  = strtok( NULL, " " );
    char* email     = strtok( NULL, " " );

    if( id_string == NULL || username == NULL || email == NULL )
    {
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi( id_string );

    if( id < 0 )
    {
        return PREPARE_NEGATIVE_ID;
    }

    if( strlen( username ) > COLUMN_USERNAME_SIZE )
    {
        return PREPARE_STRING_TOO_LONG;
    }

    if( strlen( email ) > COLUMN_EMAIL_SIZE )
    {
        return PREPARE_STRING_TOO_LONG;
    }

    statement->row_to_insert.id = id;
    strcpy( statement->row_to_insert.username, username );
    strcpy( statement->row_to_insert.email, email );

    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement( InputBuffer* input_buffer, Statement* statement )
{
    // Compare string results to how we want to handle SQL statements.
    // Here we apply the statement type to the passed in Statement.
    if( strncmp( input_buffer->buffer, "insert", 6 ) == 0 )
    {
        return prepare_insert( input_buffer, statement );
    }

    if( strncmp( input_buffer->buffer, "select", 6 ) == 0 )
    {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert( Statement* statement, Table* table )
{
    if( table->num_rows >= TABLE_MAX_ROWS )
    {
        return EXECUTE_TABLE_FULL;
    }

    Row* row_to_insert = &( statement->row_to_insert );
    serialize_row( row_to_insert, row_slot( table->pager, table->num_rows ) );
    table->num_rows++; // Add to number of rows in our table.

    // serialize_row(row_to_insert, row_slot(table->pager, table->num_rows));
    // table->num_rows++; // Add to number of rows in our table.

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select( Statement* statement, Table* table )
{
    if( table->num_rows == 0 )
    {
        log_info( "No data in table." );
    }
    else
    {
        Row row = { 0 };

        for( uint32_t i = 0; i < table->num_rows; ++i )
        {
            deserialize_row( row_slot( table->pager, i ), &row );
            log_info( "Selected Row - %s", row_to_str( &row ) );
        }
    }

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement( Statement* statement, Table* table )
{
    switch( statement->type )
    {
        case( STATEMENT_INSERT ):
            return execute_insert( statement, table );
        case( STATEMENT_SELECT ):
            return execute_select( statement, table );
    }
}

int main( int argc, char* argv[] )
{
    if( argc < 2 )
    {
        log_fatal( "Must supply a database name." );
        exit( EXIT_FAILURE );
    }

    if( !setup_logger( LOGGING_PATH_TRACE, LOG_TRACE, true ) || !setup_logger( LOGGING_PATH_INFO, LOG_INFO, false ) ||
        !setup_logger( LOGGING_PATH_FATAL, LOG_FATAL, false ) ) // trace does print to STDOUT, info does
    {
        return 1;
    }

    char* dbFilename          = argv[1];
    InputBuffer* input_buffer = new_intput_buffer();
    Table* table              = db_open( dbFilename );

    while( true ) // Main loop
    {
        read_input( input_buffer );

        if( input_buffer->buffer[0] == '.' )
        {
            switch( execute_meta_command( input_buffer, table ) )
            {
                case( META_COMMAND_SUCCESS ):
                    continue;
                case( META_COMMAND_UNRECOGNIZED_COMMAND ):
                    log_info( "Unrecognized meta command '%s'.", input_buffer->buffer );
                    continue;
            }
        }

        Statement statement = { 0 };

        switch( prepare_statement( input_buffer, &statement ) )
        {
            case( PREPARE_SUCCESS ):
                break;
            case( PREPARE_NEGATIVE_ID ):
                log_info( "Syntax error with command '%s'. ID must be positive.", input_buffer->buffer );
                continue;
            case( PREPARE_STRING_TOO_LONG ):
                log_info( "Syntax error with command '%s'. String too long.", input_buffer->buffer );
                continue;
            case( PREPARE_SYNTAX_ERROR ):
                log_info( "Syntax error with command '%s'", input_buffer->buffer );
                continue;
            case( PREPARE_UNRECOGNIZED_STATEMENT ):
                log_info( "Unrecognized keyword at start of '%s'.", input_buffer->buffer );
                continue;
        }

        // Preform lots of preparations before executing the command. This is important in state control
        // to mitigate error and preform all error checking before execution.

        switch( execute_statement( &statement, table ) )
        {
            case( EXECUTE_SUCCESS ):
                log_trace( "Command '%s' executed successfully.", input_buffer->buffer );
                break;
            case( EXECUTE_TABLE_FULL ):
                log_info( "Error: command execution failed. Table full." );
                continue;
        }
    }

    return 0;
}