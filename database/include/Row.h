#ifndef ROW_H
#define ROW_H

#include "defines.h"
#include <stdint.h>

typedef struct
{
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;

const char *row_to_str(Row *row);

void serialize_row(Row *source, void *destination);
void deserialize_row(void *source, Row *destination);

#endif // ROW_H