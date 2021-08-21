#ifndef MACROS_H
#define MACROS_H

#define BUFFER_SIZE 1024

// this is made for strings that are relaively shorter.
// this is mainly to avoid the error of directive writing to many
// bytes in a region of smaller size
#define HALF_BUFFER_SIZE 512

#define PORT 8080

struct User_state
{
	bool idVerified;
	bool accVerified;
	bool passVerified;
	bool isLoggedIn;
};

struct User_state user_state;

struct User_info
{
	char userId[HALF_BUFFER_SIZE];
	char userAcc[HALF_BUFFER_SIZE];
};

struct User_info user_info;

char transfer_type;

struct Stor_state
{
	int stor_type;
	char file_name[HALF_BUFFER_SIZE];
};

struct Stor_state stor_state;

#endif