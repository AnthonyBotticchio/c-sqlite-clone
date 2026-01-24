#ifndef DEFINES_H
#define DEFINES_H

// Common

#define PROMPT "db > "
#define size_of_attribute( Struct, Attribute ) sizeof( ( (Struct*)0 )->Attribute )

// Row

#define COLUMN_USERNAME_SIZE 64
#define COLUMN_EMAIL_SIZE 255

#define ID_SIZE size_of_attribute( Row, id )
#define USERNAME_SIZE size_of_attribute( Row, username )
#define EMAIL_SIZE size_of_attribute( Row, email )
#define ROW_SIZE ( ID_SIZE + USERNAME_SIZE + EMAIL_SIZE )

#define ID_OFFSET 0
#define USERNAME_OFFSET ( ID_OFFSET + ID_SIZE )
#define EMAIL_OFFSET ( USERNAME_OFFSET + USERNAME_SIZE )

// Table

#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 10000
#define ROWS_PER_PAGE ( PAGE_SIZE / ROW_SIZE )
#define TABLE_MAX_ROWS ( ROWS_PER_PAGE * TABLE_MAX_PAGES )

#endif // DEFINES_H