#include "Table.h"

#include "Row.h"

#include <errno.h> // For linux
#include <fcntl.h>
#include <log.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

Pager *pager_open(const char *filename)
{
    int fileDescriptor = open(filename,
                              O_RDWR |     // Read/Write mode
                                  O_CREAT, // Create file if it does not exist
                              S_IWUSR |    // User write permission
                                  S_IRUSR  // User read permission
    );

    if (fileDescriptor == -1)
    {
        log_fatal("Unable to open db file: %s", filename);
        exit(EXIT_FAILURE);
    }

    // Find file size
    struct stat st;
    if (fstat(fileDescriptor, &st) == -1)
    {
        log_fatal("Unable to get file size: %s", filename);
        exit(EXIT_FAILURE);
    }

    Pager *pager = malloc(sizeof(Pager));
    *pager = (Pager){
        .file_descriptor = fileDescriptor,
        .file_size = st.st_size};

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
    {
        pager->pages[i] = NULL; // set all pages to NULL
    }

    return pager;
}

void free_pager(Pager *pager)
{
    for (int i = 0; pager->pages[i]; i++)
    {
        free(pager->pages[i]);
    }

    free(pager);
}

void *get_page(Pager *pager, uint32_t page_num)
{
    if (page_num > TABLE_MAX_PAGES)
    {
        log_fatal("Tried to fetch page number out of bounds. %d > &d.", page_num, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == NULL)
    {
        // Cache miss. Allocate memory and load from file.
        void *page = malloc(PAGE_SIZE);
        uint32_t num_pages = pager->file_size / PAGE_SIZE;

        // We might save a partial page at the end of the file
        if (pager->file_size % PAGE_SIZE)
        { // If we are at the beginning of a page (returns 0)
            num_pages++;
        }

        if (page_num <= num_pages)
        {
            lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
            size_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);

            if (bytes_read == -1)
            {
                log_fatal("Error reading file: %d", errno); // Logs error number
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[page_num] = page;
    }

    return pager->pages[page_num];
}

void pager_flush(Pager *pager, uint32_t page_num, size_t size)
{
    // We need to take the pager's file_descriptor and write to the file all the bytes at that page_num
    // We need to make use of the offsets within the file to know page_num * PAGE_OFFSET

    if (pager->pages[page_num] == NULL)
    {
        log_fatal("Tried to flush a null page. Page num: %d", page_num);
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET); // If we are able to seek to the point in memory of 'page_num * PAGE_SIZE'

    if (offset == -1)
    {
        log_fatal("Error seeking: %d", errno);
        exit(EXIT_FAILURE);
    }

    size_t bytes_written = write(pager->file_descriptor, pager->pages[page_num], size);

    if (bytes_written == -1)
    {
        log_fatal("Error writing bytes to file: %d", errno);
        exit(EXIT_FAILURE);
    }
}

Table *db_open(const char *filename)
{
    Pager *pager = pager_open(filename);
    uint32_t num_rows = pager->file_size / ROW_SIZE;
    log_info("Database opened with '%d' rows loaded.", num_rows);

    Table *table = malloc(sizeof(Table));
    *table = (Table){
        .pager = pager,
        .num_rows = num_rows};

    return table;
}

void db_close(Table *table)
{
    Pager *pager = table->pager;
    uint32_t num_full_pages = table->num_rows / ROWS_PER_PAGE;

    for (uint32_t i = 0; i < num_full_pages; i++)
    {
        if (pager->pages[i] == NULL)
        {
            continue;
        }

        pager_flush(pager, i, PAGE_SIZE);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    // There is a case where we need to save a partial page
    // This will not be the case once the table is switched to a B-tree.
    uint32_t num_additional_rows = table->num_rows % ROWS_PER_PAGE;
    if (num_additional_rows > 0)
    {
        uint32_t lastPage = num_full_pages;
        if (pager->pages[lastPage] != NULL)
        {
            pager_flush(pager, lastPage, num_additional_rows * ROW_SIZE);
            free(pager->pages[lastPage]);
            pager->pages[lastPage] = NULL;
        }
    }

    int result = close(pager->file_descriptor);

    if (result == -1)
    {
        log_fatal("Error closing db file.");
        exit(EXIT_FAILURE);
    }

    // Loop through all pages to make sure they are null before freeing the pager itself
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
    {
        if (pager->pages[i])
        {
            log_fatal("Warning! Page at index: %d was not free'd or flushed properly. It is not NULL.", i);
            free(pager->pages[i]);
            pager->pages[i] = NULL;
        }
    }

    // Free both the pager and table
    free(pager);
    free(table);
}
