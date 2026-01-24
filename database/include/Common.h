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
    StatementType type;
    Row row_to_insert;
} Statement;

/// @brief Returns a pointer to the memory slot for the specified row in the table.
/// @param pager Pointer to the pager structure containing the pages, which contain rows.
/// @param row_num The index of the row whose slot is to be retrieved.
/// @return Pointer to the memory location of the specified row.
extern void* row_slot( Pager* pager, uint32_t row_num );

#endif // COMMON_H
