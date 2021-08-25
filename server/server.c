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

int main(int argc, char *argv[])
{

    // database setup
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *db_err;
    sqlite3_open("users.db", &db);
    int res = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users(id varchar(512), acc varchar(512), pass varchar(512));", NULL, NULL, &db_err);
    if (res != SQLITE_OK)
    {
        printf("error: %s\n", db_err);
    }

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
    struct User_info user_info;
    struct Stor_state stor_state;
    reset_state();

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0)
    {
        perror("Socket Failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Bind Failed");
        exit(1);
    }

    while (1)
    {
        if (is_closed)
        {
            if (listen(server_fd, 5) < 0)
            {
                perror("Listen Failed");
                exit(1);
            }

            client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_addr_len);

            if (client_fd < 0)
            {
                perror("Failed Accept");
                exit(1);
            }

            strcpy(message, "+CS725 SFTP Service");
            send(client_fd, message, BUFFER_SIZE, 0);
            memset(message, 0, BUFFER_SIZE);
            printf("Connection established with client\n");

            is_closed = false;
        }

        memset(temp, 0, BUFFER_SIZE);

        read(client_fd, buffer, BUFFER_SIZE);
        printf("command recieved: %s\n", buffer);

        strncpy(temp, buffer, 4);
        temp[4] = 0;

        if (strcmp("DONE", temp) == 0)
        {
            done(client_fd, message, buffer, &is_closed);
        }
        else if (strcmp("USER", temp) == 0)
        {
            user(client_fd, db, stmt, message, buffer);
        }
        else if (strcmp("ACCT", temp) == 0)
        {
            u_acct(client_fd, db, stmt, message, buffer);
        }
        else if (strcmp("PASS", temp) == 0)
        {
            pass(client_fd, db, stmt, message, buffer);
        }
        else if (strcmp("TYPE", temp) == 0)
        {
            type(client_fd, message, buffer);
        }
        else if (strcmp("STOR", temp) == 0)
        {
            stor(client_fd, message, buffer);
        }
        else if (strcmp("SIZE", temp) == 0)
        {
            size(client_fd, message, buffer);
        }
        else if (strcmp("KILL", temp) == 0)
        {
            kill(client_fd, message, buffer);
        }
        else if (strcmp("LIST", temp) == 0)
        {
            list(client_fd, message, buffer);
        }
        else if (strcmp("NAME", temp) == 0)
        {
            name(client_fd, message, buffer);
        }
        else if (strcmp("TOBE", temp) == 0)
        {
            tobe(client_fd, message, buffer);
        }
        else if (strcmp("RETR", temp) == 0)
        {
            retr(client_fd, message, buffer);
        }
        else if (strcmp("SEND", temp) == 0)
        {
            send_retr(client_fd, message, buffer);
        }
        else
        {
            strcat(message, "-Invalid Command");
            send(client_fd, message, BUFFER_SIZE, 0);
            memset(buffer, 0, BUFFER_SIZE);
            memset(message, 0, BUFFER_SIZE);
        }
        memset(buffer, 0, BUFFER_SIZE);
        memset(message, 0, BUFFER_SIZE);
    }
    return 0;
}
