#include "Common.h"

#include <log.h>
#include <stdlib.h>

void* row_slot( Pager* pager, uint32_t row_num )
{
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page        = get_page( pager, page_num );

    uint32_t row_offset  = row_num % ROWS_PER_PAGE; // A table can have multiple pages. We need to know which row in that page
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}