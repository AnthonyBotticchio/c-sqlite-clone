#ifndef COMMON_H
#define COMMON_H

#include "CommandEnums.h"
#include "Row.h"
#include "Table.h"

#include <stdint.h>

// Linux
#include <stddef.h> 

typedef struct
{
    char *buffer;
    size_t buffer_size;
    size_t input_length; // 's' stands for signed size_t which means that ssize_t can return negative values
} InputBuffer;

typedef struct
{
    StatementType type;
    Row row_to_insert;
} Statement;

InputBuffer *new_intput_buffer();
void free_input_buffer(InputBuffer *input_buffer);
void read_input(InputBuffer *input_buffer);

/// @brief Returns a pointer to the memory slot for the specified row in the table.
/// @param table Pointer to the Table structure containing the rows.
/// @param row_num The index of the row whose slot is to be retrieved.
/// @return Pointer to the memory location of the specified row.
void *row_slot(Pager *pager, uint32_t row_num);

#endif // COMMON_H
