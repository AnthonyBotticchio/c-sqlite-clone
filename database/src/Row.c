#include "Row.h"

#include <stdio.h>
#include <string.h>

const char *row_to_str(Row *row)
{
    static char buffer[512]; // WARNING! Not thread safe!
    snprintf(buffer, sizeof(buffer), "ID: %d, Username: %s, Email: %s", row->id, row->username, row->email);
    return buffer;
}

void serialize_row(Row *source, void *destination)
{
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    strncpy(destination + USERNAME_OFFSET, source->username, USERNAME_SIZE);
    strncpy(destination + EMAIL_OFFSET, source->email, EMAIL_SIZE);
}

void deserialize_row(void *source, Row *destination)
{
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    strncpy(destination->username, source + USERNAME_OFFSET, USERNAME_SIZE);
    strncpy(destination->email, source + EMAIL_OFFSET, EMAIL_SIZE);
}