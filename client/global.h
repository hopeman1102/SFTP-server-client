#ifndef MACROS_H
#define MACROS_H

#define BUFFER_SIZE 1024

// this is made for strings that are relaively shorter.
// this is mainly to avoid the error of directive writing to many
// bytes in a region of smaller size
#define HALF_BUFFER_SIZE 512

#define PORT 8080

struct Stor_state
{
	int stor_type;
	char file_name[HALF_BUFFER_SIZE];
};

struct Stor_state stor_state;

#endif