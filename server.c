#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char *argv[]) {
    int server_fd, client_fd, read_val;
    struct sockaddr_in serv_addr, cli_addr;
    int cli_addr_len = sizeof(cli_addr);
    char buffer[1024] = {0};
    char *message = "HELLO";
    
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

    if (listen(server_fd, 5) < 0) {
        perror("Listen Failed");
        exit(1);
    }

    client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_addr_len);

    if (client_fd < 0) {
       perror("Failed Accept");
       exit(1);
    }

    read_val = read(client_fd, buffer, 1024);
    printf("%s\n", buffer);
    send(client_fd, message, strlen(message), 0);
    printf("Message Sent");

    return 0;
}    
