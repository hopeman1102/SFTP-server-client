#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include "sqlite/sqlite3.h"
#include "global.h"

void clear_buffers(char *message, char *buffer)
{
	memset(message, 0, BUFFER_SIZE);
	memset(buffer, 0, BUFFER_SIZE);
}

bool is_file_present(char *file_name)
{
	struct stat buffer;
	char file_addr[BUFFER_SIZE] = {0};
	sprintf(file_addr, "%s/%s", dir, file_name);
	int exist = stat(file_addr, &buffer);
	if (exist == 0)
		return true;
	else
		return false;
}

long int findSize(char *file_name, char *temp_dir)
{
	struct stat buffer;
	char file_addr[BUFFER_SIZE] = {0};
	sprintf(file_addr, "%s/%s", temp_dir, file_name);

	FILE *file = fopen(file_addr, "r");

	if (file == NULL)
	{
		printf("file not found\n");
		return -1;
	}

	fseek(file, 0L, SEEK_END);
	long int res = ftell(file);
	fclose(file);
	return res;
}

void stor_file(int sockfd, int size)
{
	int n;
	FILE *fp;
	char file_addr[BUFFER_SIZE+10] = {0};
	if (is_file_present(stor_state.file_name) && stor_state.stor_type == 0)
	{
		sprintf(file_addr, "%s/_%s", dir, stor_state.file_name);
	}
	else
	{
		sprintf(file_addr, "%s/%s", dir, stor_state.file_name);
	}
	int stor_buffer_size = 1;
	char buffer[stor_buffer_size + 1];
	buffer[stor_buffer_size] = 0;

	if (is_file_present(stor_state.file_name) && stor_state.stor_type == 2)
	{
		fp = fopen(file_addr, "a");
	}
	else
	{
		fp = fopen(file_addr, "w");
	}

	for (int i = 0; i < size; i++)
	{
		n = recv(sockfd, buffer, stor_buffer_size, 0);
		if (n <= 0)
		{
			break;
			return;
		}
		fprintf(fp, "%s", buffer);
		bzero(buffer, stor_buffer_size);
	}
	fclose(fp);
	return;
}

void send_file(FILE *fp, int sockfd, long int size)
{
	int n;
	int send_buffer_size = 1;
	char data[send_buffer_size + 1];
	data[send_buffer_size] = 0;
	int i = 0;
	for (int i = 0; i < size; i++)
	{
		fread(data, 1, 1, fp);
		if (send(sockfd, data, send_buffer_size, 0) == -1)
		{
			printf("err: error in sending file");
		}
		bzero(data, send_buffer_size);
	}
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
	strcpy(file_to_change_name, "");
	strcpy(retr_file_name, "");
	retr_file_size = 0;
	strcpy(dir, "recieved_files");
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

bool is_pass_present(sqlite3 *db, sqlite3_stmt *stmt, char *user_id)
{
	char query[512];
	sprintf(query, "select id, acc, pass from users where id = '%s';", user_id);
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);
	const unsigned char *pass_temp;
	sqlite3_step(stmt);
	pass_temp = sqlite3_column_text(stmt, 2);
	if (pass_temp != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_acct_present(sqlite3 *db, sqlite3_stmt *stmt, char *user_id)
{
	char query[512];
	sprintf(query, "select id, acc, pass from users where id = '%s';", user_id);
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);
	const unsigned char *acct_temp;
	sqlite3_step(stmt);
	acct_temp = sqlite3_column_text(stmt, 1);
	if (acct_temp != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
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
		bool is_pass = is_pass_present(db, stmt, user_id);
		bool is_acct = is_acct_present(db, stmt, user_id);
		if (is_pass && is_acct)
		{
			sprintf(message, "+%s ok, send account and password", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
		else if (!is_pass && is_acct)
		{
			user_state.passVerified = true;
			sprintf(message, "+%s ok, send account, password not required", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
		else if (!is_acct && is_pass)
		{
			user_state.accVerified = true;
			sprintf(message, "+%s ok, send password, account not required", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
		else
		{
			user_state.passVerified = true;
			user_state.accVerified = true;
			user_state.isLoggedIn = true;
			sprintf(message, "!%s logged in", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
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

	char query[1024];
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

	char query[1024];
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
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

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
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	// add check to make sure stor was run first
	char *num_of_bytes_str = strtok(buffer, " ");
	num_of_bytes_str = strtok(NULL, " ");
	int num_of_bytes = atoi(num_of_bytes_str);

	// add code for free space

	strcpy(message, "+ok, waiting for file");
	send(client_fd, message, strlen(message), 0);

	clear_buffers(message, buffer);

	stor_file(client_fd, num_of_bytes);

	sprintf(message, "+Saved %s", stor_state.file_name);
	printf("%s\n", message);
	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
}

void kill(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	char *file_name = strtok(buffer, " ");
	file_name = strtok(NULL, " ");
	if (file_name == NULL)
	{
		strcpy(message, "-file not specified");
	}
	else
	{
		char file_addr[BUFFER_SIZE+10] = {0};
		sprintf(file_addr, "%s/%s", dir, file_name);
		if (remove(file_addr) == 0)
		{
			sprintf(message, "+%s deleted", file_name);
		}
		else
		{
			strcpy(message, "-file not deleted");
		}
	}

	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
}

void list(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	char *list_type = strtok(buffer, " ");
	list_type = strtok(NULL, " ");

	char *subdir;
	subdir = strtok(NULL, " ");

	struct dirent *de;
	char temp_dir[BUFFER_SIZE] = {0};

	if (subdir == NULL)
	{
		sprintf(temp_dir, "./%s", dir);
	}
	else
	{
		sprintf(temp_dir, "./%s/%s", dir, subdir);
	}
	
	
	DIR *dr = opendir(temp_dir);
	char temp_str[HALF_BUFFER_SIZE] = {0};

	if (list_type == NULL)
	{
		strcpy(message, "-list type not specified");
	}
	else
	{
		if (dr == NULL)
		{
			strcpy(message, "-error in opening directory");
		}
		else
		{
			strcpy(message, "+PS\n");
		}
		if (strcmp(list_type, "F") == 0)
		{
			while ((de = readdir(dr)) != NULL)
			{
				if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
				{
					sprintf(temp_str, "%s\n", de->d_name);
					strcat(message, temp_str);
				}
			}
		}
		else if (strcmp(list_type, "V") == 0)
		{
			while ((de = readdir(dr)) != NULL)
			{
				if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
				{
					long int file_size = findSize(de->d_name, temp_dir);
					printf("%s\n", de->d_name);
					sprintf(temp_str, "%s\t%ld bytes\n", de->d_name, file_size);
					strcat(message, temp_str);
				}
			}
		}
		else
		{
			strcpy(message, "invalid LIST type");
		}
	}
	closedir(dr);
	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
}

void name(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	char *file_name = strtok(buffer, " ");
	file_name = strtok(NULL, " ");

	if (file_name == NULL)
	{
		strcpy(message, "-file name not specified");
	}
	else
	{
		if (is_file_present(file_name))
		{
			strcpy(file_to_change_name, file_name);
			strcpy(message, "+File exists");
		}
		else
		{
			sprintf(message, "-Can't find %s", file_name);
		}
	}
	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
}

void tobe(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	char *new_file_spec = strtok(buffer, " ");
	new_file_spec = strtok(NULL, " ");

	if (new_file_spec == NULL)
	{
		strcpy(message, "-new file spec not specified");
	}
	else
	{
		char file_addr[BUFFER_SIZE] = {0};
		char file_addr_new[BUFFER_SIZE] = {0};
		sprintf(file_addr, "%s/%s", dir, file_to_change_name);
		sprintf(file_addr_new, "%s/%s", dir, new_file_spec);
		int res = rename(file_addr, file_addr_new);
		if (res == 0)
		{
			sprintf(message, "+%s renamed to %s", file_to_change_name, new_file_spec);
		}
		else
		{
			strcpy(message, "-File wasn't renamed");
		}
	}
	strcpy(file_to_change_name, "");
	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
}

void retr(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	char *file_spec = strtok(buffer, " ");
	file_spec = strtok(NULL, " ");

	if (file_spec == NULL)
	{
		strcpy(message, "-File doesn't exist");
	}
	else
	{
		if (is_file_present(file_spec))
		{
			sprintf(message, "%ld", findSize(file_spec, dir));
			retr_file_size = findSize(file_spec, dir);
			strcpy(retr_file_name, file_spec);
		}
		else
		{
			strcpy(message, "-File doesn't exist");
		}
	}

	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
}

void send_retr(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	FILE *fp;
	char file_addr[BUFFER_SIZE] = {0};
	sprintf(file_addr, "%s/%s", dir, retr_file_name);
	fp = fopen(file_addr, "r");
	send_file(fp, client_fd, retr_file_size);
}

void stop_retr(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	strcpy(message, "+ok, RETR aborted");
	strcpy(retr_file_name, "");
	send(client_fd, message, strlen(message), 0);
	retr_file_size = 0;
	clear_buffers(message, buffer);
}

void cdir(int client_fd, char *message, char *buffer)
{
	if (!user_state.isLoggedIn)
	{
		sprintf(message, "-Permission denied, not logged in");
		send(client_fd, message, strlen(message), 0);
		clear_buffers(message, buffer);
		return;
	}

	char *dir_str = strtok(buffer, " ");
	dir_str = strtok(NULL, " ");
	if (strcmp(dir_str, "..") == 0)
	{
		sprintf(dir, "recieved_files");
		sprintf(message, "!Changed working dir to root/parent dir");
	}
	else
	{
		char dir_str_addr[HALF_BUFFER_SIZE] = {0};
		sprintf(dir_str_addr, "recieved_files/%s", dir_str);
		DIR *dir_fd = opendir(dir_str_addr);
		if (dir_fd)
		{
			strcpy(dir, dir_str_addr);
			closedir(dir_fd);
			sprintf(message, "!Changed working dir to %s", dir_str);
		}
		else if (ENOENT == errno)
		{
			strcpy(message, "-Can't connect to directory because: specified directory does not exist");
		}
		else
		{
			strcpy(message, "-Can't connect to directory");
		}
	}

	send(client_fd, message, strlen(message), 0);
	clear_buffers(message, buffer);
}