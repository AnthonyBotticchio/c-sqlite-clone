#ifndef META_COMMAND_H
#define META_COMMAND_H

#include "InputBuffer.h"

/// @brief Non-SQL statements like .exit are called “meta-commands”. They all start with a dot. We handle them separately.
typedef enum
{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

extern MetaCommandResult execute_meta_command( InputBuffer* input_buffer, Table* table );

#endif // META_COMMAND_H