#ifndef COMMAND_ENUMS_H
#define COMMAND_ENUMS_H

typedef enum
{
    PREPARE_SUCCESS,
    PREPARE_NEGATIVE_ID,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_TOO_LONG,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum
{
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

/// @brief Base options for SQL statements
typedef enum
{
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

#endif // COMMAND_ENUMS_H