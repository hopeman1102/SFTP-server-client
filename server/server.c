#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "database.h"
#define PORT 8080

int main(int argc, char *argv[]) {


    //---------------------------------
    char* userID = "user1";
    char userPass[1024] = {0};
    int temp = getPassword(userID, userPass);
    printf("password: %s\n", userPass);
    //---------------------------------

    int server_fd, client_fd, read_val;
    struct sockaddr_in serv_addr, cli_addr;
    int cli_addr_len = sizeof(cli_addr);
    char buffer[1024] = {0};
    char message[1024] = {0};
    bool isClosed = true;
        
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        perror("Socket Failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind Failed");
        exit(1);
    }

    while (1) {
        if (isClosed) {
            if (listen(server_fd, 5) < 0) {
                perror("Listen Failed");
                exit(1);
            }

            client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_addr_len);

            if (client_fd < 0) {
                perror("Failed Accept");
                exit(1);
            }
            isClosed = false;
        }

        read(client_fd, buffer, 1024);
        if (strcmp("DONE", buffer) == 0) {
            strcpy(message, "+");
            send(client_fd, message, strlen(message), 0);
            memset(message, 0, sizeof(message));
            memset(buffer, 0, sizeof(buffer));
            if (close(client_fd) < 0) {
                perror("Failed DONE command for client_fd");
                exit(1);
            }
            printf("Connection Closed\n");
            isClosed = true;
        } else {
            printf("%s\n", buffer);
            strcat(buffer, ", wassap");
            send(client_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            printf("Message Sent\n");
        }
    }
    return 0;
}    
