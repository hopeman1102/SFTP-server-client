#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "macros.h"

void done(int client_fd, char* message, char* buffer, bool* is_closed) {
	printf("hello\n");
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