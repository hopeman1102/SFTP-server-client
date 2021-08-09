#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "sqlite/sqlite3.h"
#include "sftp.h"
#include "database.h"
#include "macros.h"

int main(int argc, char *argv[]) {

    // database setup
    sqlite3* db;
    sqlite3_stmt* smt;
    char* db_err;
    sqlite3_open("users.db", &db);
    int res = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users(id varchar(512), pass varchar(512));", NULL, NULL, &db_err);
    if (res != SQLITE_OK) {
        printf("error: %s\n", db_err);
    }
 
    // server setup
    int server_fd, client_fd, read_val;
    struct sockaddr_in serv_addr, cli_addr;
    int cli_addr_len = sizeof(cli_addr);
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    bool is_closed = true;
       
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
        if (is_closed) {
            if (listen(server_fd, 5) < 0) {
                perror("Listen Failed");
                exit(1);
            }

            client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_addr_len);

            if (client_fd < 0) {
                perror("Failed Accept");
                exit(1);
            }
            is_closed = false;
        }

        read(client_fd, buffer, BUFFER_SIZE);
        if (strcmp("DONE", buffer) == 0) {
            done(client_fd, &message, &buffer, &is_closed);
        } else {
            printf("%s\n", buffer);
            strcat(buffer, ", wassap");
            send(client_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            printf("Message Sent\n");
        }
    }
    return 0;
}    
