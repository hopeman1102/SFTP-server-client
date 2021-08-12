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
#include "global.h"

int main(int argc, char *argv[]) {

    // database setup
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* db_err;
    sqlite3_open("users.db", &db);
    int res = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users(id varchar(512), acc varchar(512), pass varchar(512));", NULL, NULL, &db_err);
    if (res != SQLITE_OK) {
        printf("error: %s\n", db_err);
    }

    // char query[512];
    // sprintf(query, "insert into users VALUES ('user2', 'u2acc2', 'pass123')");
    // res = sqlite3_exec(db, query, NULL, NULL, &db_err);
    // if (res != SQLITE_OK) {
    //     printf("error: %s\n", db_err);
    // }
 
    // server setup
    int server_fd, client_fd, read_val;
    struct sockaddr_in serv_addr, cli_addr;
    int cli_addr_len = sizeof(cli_addr);
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    bool is_closed = true;
    char temp[BUFFER_SIZE] = {0};

    // initialising global variables
    struct User_state user_state;
    user_state.accVerified = false;
    user_state.idVerified = false;
    user_state.isLoggedIn = false;
    user_state.passVerified = false;
    struct User_info user_info;
    strcpy(user_info.userId, "");
    strcpy(user_info.userAcc, "");

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

        memset(temp, 0, BUFFER_SIZE);
        
        read(client_fd, buffer, BUFFER_SIZE);

        strncpy(temp, buffer, 4);
        temp[4] = 0;

        if (strcmp("DONE", temp) == 0) {
            done(client_fd, message, buffer, &is_closed);
        } 
        else if(strcmp("USER", temp) == 0) {
            printf("user: %s\n", temp);
            printf("buffer: %s\n", buffer);
            user(client_fd, db, stmt, message, buffer);
        } 
        else {
            strcat(buffer, ", wassap");
            send(client_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            printf("Message Sent\n");
        }
    }
    return 0;
}    
