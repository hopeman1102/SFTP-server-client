#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "sqlite/sqlite3.h"
#include "global.h"

void done(int client_fd, char* message, char* buffer, bool* is_closed) {
	strcpy(message, "+");
	send(client_fd, message, strlen(message), 0);
	memset(message, 0, BUFFER_SIZE);
	memset(buffer, 0, BUFFER_SIZE);
	if (close(client_fd) < 0) {
		perror("Failed DONE command for client_fd");
		exit(1);
	}
	printf("Connection Closed\n");
	*is_closed = true;
}

void user(int client_fd, sqlite3* db, sqlite3_stmt* stmt, char* message, char* buffer) {
	char temp[BUFFER_SIZE] = {0};
	char* user_id = strtok(buffer, " ");
	user_id = strtok(NULL, " ");

	char query[512];
	sprintf(query, "select id, acc, pass from users where id = '%s';", user_id);
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);
	const unsigned char* id_temp;
	sqlite3_step(stmt);
	id_temp = sqlite3_column_text(stmt, 0);
	if (id_temp != NULL) {
		user_state.idVerified = true;
		strcpy(user_info.userId, (char*) id_temp);
		sprintf(message, "+%s ok, send account and password", user_info.userId);
		send(client_fd, message, strlen(message), 0);
	}
	else {
		sprintf(message, "-%s user id not found", user_id);
		send(client_fd, message, strlen(message), 0);
	}
	memset(message, 0, BUFFER_SIZE);
	memset(buffer, 0, BUFFER_SIZE);
}

void u_acct(int client_fd, sqlite3* db, sqlite3_stmt* stmt, char* message, char* buffer) {
	char temp[BUFFER_SIZE] = {0};
	char* user_acct = strtok(buffer, " ");
	user_acct = strtok(NULL, " ");

	char query[512];
	sprintf(query, "select id, acc, pass from users where id = '%s' AND acc = '%s';", user_info.userId, user_acct);
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);
	const unsigned char* acc_temp;
	sqlite3_step(stmt);
	acc_temp = sqlite3_column_text(stmt, 1);
	if (acc_temp != NULL) {
		user_state.accVerified = true;
		strcpy(user_info.userAcc, (char*) acc_temp);
		if (user_state.passVerified) {
			user_state.isLoggedIn = true;
			sprintf(message, "!%s logged in", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		} else {
			sprintf(message, "+%s account verified, send password", user_info.userId);
			send(client_fd, message, strlen(message), 0);
		}
	}
	else {
		sprintf(message, "-%s invalid account", user_acct);
		send(client_fd, message, strlen(message), 0);
	}
	memset(message, 0, BUFFER_SIZE);
	memset(buffer, 0, BUFFER_SIZE);
}