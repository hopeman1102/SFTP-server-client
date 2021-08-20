#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "sqlite/sqlite3.h"
#include "global.h"

void clear_buffers(char *message, char *buffer)
{
	memset(message, 0, BUFFER_SIZE);
	memset(buffer, 0, BUFFER_SIZE);
}

bool is_file_present(char *file_name)
{
	FILE *fp;
	char file_addr[BUFFER_SIZE] = {0};
	sprintf(file_addr, "recieved_files/%s", file_name);
	fp = fopen(file_addr, "r");
	if (fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

void reset_state()
{
	user_state.accVerified = false;
	user_state.idVerified = false;
	user_state.isLoggedIn = false;
	user_state.passVerified = false;
	strcpy(user_info.userId, "");
	strcpy(user_info.userAcc, "");
	stor_state.stor_type = 0;
	strcpy(stor_state.file_name, "");
}

void done(int client_fd, char *message, char *buffer, bool *is_closed)
{
	strcpy(message, "+");
	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
	if (close(client_fd) < 0)
	{
		perror("Failed DONE command for client_fd");
		exit(1);
	}
	printf("Connection Closed\n");
	*is_closed = true;
	reset_state();
}

void user(int client_fd, sqlite3 *db, sqlite3_stmt *stmt, char *message, char *buffer)
{
	char *user_id = strtok(buffer, " ");
	user_id = strtok(NULL, " ");

	char query[512];
	sprintf(query, "select id, acc, pass from users where id = '%s';", user_id);
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);
	const unsigned char *id_temp;
	sqlite3_step(stmt);
	id_temp = sqlite3_column_text(stmt, 0);
	if (id_temp != NULL)
	{
		user_state.idVerified = true;
		strcpy(user_info.userId, (char *)id_temp);
		sprintf(message, "+%s ok, send account and password", user_info.userId);
		send(client_fd, message, strlen(message), 0);
	}
	else
	{
		sprintf(message, "-%s user id not found", user_id);
		send(client_fd, message, strlen(message), 0);
	}
	clear_buffers(message, buffer);
}

void u_acct(int client_fd, sqlite3 *db, sqlite3_stmt *stmt, char *message, char *buffer)
{
	char *user_acct = strtok(buffer, " ");
	user_acct = strtok(NULL, " ");

	char query[512];
	sprintf(query, "select id, acc, pass from users where id = '%s' AND acc = '%s';", user_info.userId, user_acct);
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);
	const unsigned char *acc_temp;
	sqlite3_step(stmt);
	acc_temp = sqlite3_column_text(stmt, 1);
	if (acc_temp != NULL)
	{
		user_state.accVerified = true;
		strcpy(user_info.userAcc, (char *)acc_temp);
		if (user_state.passVerified)
		{
			user_state.isLoggedIn = true;
			sprintf(message, "!%s logged in", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
		else
		{
			sprintf(message, "+%s account verified, send password", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
	}
	else
	{
		sprintf(message, "-%s invalid account", user_acct);
		send(client_fd, message, strlen(message), 0);
	}
	clear_buffers(message, buffer);
}

void pass(int client_fd, sqlite3 *db, sqlite3_stmt *stmt, char *message, char *buffer)
{
	char *user_pass = strtok(buffer, " ");
	user_pass = strtok(NULL, " ");

	char query[512];
	sprintf(query, "select id, acc, pass from users where id = '%s' AND pass = '%s';", user_info.userId, user_pass);
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);
	const unsigned char *pass_temp;
	sqlite3_step(stmt);
	pass_temp = sqlite3_column_text(stmt, 2);
	if (pass_temp != NULL)
	{
		user_state.passVerified = true;
		if (user_state.accVerified)
		{
			user_state.isLoggedIn = true;
			sprintf(message, "!%s logged in", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
		else
		{
			sprintf(message, "+%s password verified, send account", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
	}
	else
	{
		sprintf(message, "-invalid password");
		send(client_fd, message, strlen(message), 0);
	}
	clear_buffers(message, buffer);
}

int type(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return 1;
	}

	char *type = strtok(buffer, " ");
	type = strtok(NULL, " ");

	if (strcmp("A", type) == 0)
	{
		transfer_type = 'A';
		strcpy(message, "+Using Ascii mode");
	}
	else if (strcmp("B", type) == 0)
	{
		transfer_type = 'B';
		strcpy(message, "+Using Binary mode");
	}
	else if (strcmp("C", type) == 0)
	{
		transfer_type = 'C';
		strcpy(message, "+Using Continuous mode");
	}
	else
	{
		strcpy(message, "-Type not valid");
	}
	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
	return 0;
}

void stor(int client_fd, char *message, char *buffer)
{
	char *stor_type = strtok(buffer, " ");
	stor_type = strtok(NULL, " ");
	char *file_name = strtok(NULL, " ");
	strcpy(stor_state.file_name, file_name);

	if (strcmp(stor_type, "NEW") == 0)
	{
		stor_state.stor_type = 0;
		if (is_file_present(file_name))
		{
			strcpy(message, "+File exists, will create new generation of file");
		}
		else
		{
			strcpy(message, "+File does not exist, will create new file");
		}
	}
	else if (strcmp(stor_type, "OLD") == 0)
	{
		stor_state.stor_type = 1;
		if (is_file_present(file_name))
		{
			strcpy(message, "+Will write over old file");
		}
		else
		{
			strcpy(message, "+Will create new file");
		}
	}
	else if (strcmp(stor_type, "APP") == 0)
	{
		stor_state.stor_type = 2;
		if (is_file_present(file_name))
		{
			strcpy(message, "+Will append to file");
		}
		else
		{
			strcpy(message, "+Will create file");
		}
	}
	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
}

void size(int client_fd, char *message, char *buffer)
{
	
}