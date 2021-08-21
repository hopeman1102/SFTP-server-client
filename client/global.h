#ifndef MACROS_H
#define MACROS_H

#define BUFFER_SIZE 1024
#define PORT 8080

struct Stor_state
{
	int stor_type;
	char file_name[BUFFER_SIZE];
};

struct Stor_state stor_state;

#endif