#ifndef MACROS_H
#define MACROS_H

#define BUFFER_SIZE 1024
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
	char userId[BUFFER_SIZE];
	char userAcc[BUFFER_SIZE];
};

struct User_info user_info;

char transfer_type;

#endif