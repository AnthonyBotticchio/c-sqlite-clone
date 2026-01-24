#ifndef TABLE_H
#define TABLE_H

#include "defines.h"

#include <stdint.h>

// Linux
#include <stddef.h>

typedef struct
{
    int file_descriptor;
    uint32_t file_size;
    void* pages[TABLE_MAX_PAGES]; // Generic pointers to memory that will be the pages that contain rows.
} Pager;

typedef struct
{
    uint32_t num_rows;
    Pager* pager;
} Table;

extern Pager* pager_open( const char* filename );
extern void free_pager( Pager* pager );
extern void* get_page( Pager* pager, uint32_t page_num );
extern void pager_flush( Pager* pager, uint32_t page_num, size_t size );
extern Table* db_open( const char* filename );
extern void db_close( Table* table );
extern void free_table( Table* table );

#endif // TABLE_H
