#ifndef TABLE_H
#define TABLE_H

#include "defines.h"
#include <stdint.h>
#include <stddef.h> // For linux

typedef struct
{
    int file_descriptor;
    uint32_t file_size;
    void *pages[TABLE_MAX_PAGES]; // Generic pointers to memory that will be the pages that contain rows.
} Pager;

typedef struct
{
    uint32_t num_rows;
    Pager *pager;
} Table;

Pager *pager_open(const char *filename);
void free_pager(Pager *pager);
void *get_page(Pager *pager, uint32_t page_num);
void pager_flush(Pager *pager, uint32_t page_num, size_t size);

Table *db_open(const char* filename);
void db_close(Table *table);
void free_table(Table *table);

#endif // TABLE_H
