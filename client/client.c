#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "global.h"

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE] = {0};
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        printf("Socket Failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid Address");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed");
        return -1;
    }

    read(sock, buffer, 1024);
    printf("%s\n", buffer);

    while (1) {
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = 0; // removing the \n if exists
        send(sock, message, strlen(message), 0);
        read(sock, buffer, 1024);
        if (strcmp("DONE", message) == 0 && strcmp("+", buffer) == 0) {
            if (close(sock) < 0) {
                perror("DONE command failed");
                exit(1);
            }
        }
        printf("%s\n", buffer);
        memset(message, 0, sizeof(message));
        memset(buffer, 0, sizeof(buffer));
    }
    return 0;
}
