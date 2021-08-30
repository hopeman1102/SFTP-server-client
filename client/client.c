#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h> 
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "global.h"

// checks if the file is present in the current dir
bool is_file_present(char *file_name)
{
    struct stat buffer;
    char file_addr[BUFFER_SIZE] = {0};
    sprintf(file_addr, "transfer_files/%s", file_name);
    int exist = stat(file_addr, &buffer);
    if (exist == 0)
        return true;
    else
        return false;
}

// sends the file to the server
void send_file(FILE *fp, int sockfd, int size)
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

// recieves the file from the server and saves in the dir
void stor_file(int sockfd, int size, char *file_name)
{
    int n;
    FILE *fp;
    char file_addr[BUFFER_SIZE] = {0};
    sprintf(file_addr, "transfer_files/%s", file_name);

    int stor_buffer_size = 1;
    char buffer[stor_buffer_size + 1];
    buffer[stor_buffer_size] = 0;

    fp = fopen(file_addr, "w");

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

int main(int argc, char *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE] = {0};
    char buffer[BUFFER_SIZE] = {0};
    char temp[BUFFER_SIZE] = {0};
    int retr_file_size = 0;
    char retr_file_name[HALF_BUFFER_SIZE] = {0};
    bool sent_stor = false;
    FILE *fp;

    // initialising global variables
    struct Stor_state stor_state;
    stor_state.stor_type = 0;
    strcpy(stor_state.file_name, "");

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        printf("Socket Failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid Address");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Failed");
        return -1;
    }

    read(sock, buffer, 1024);
    printf("%s\n", buffer);
    memset(message, 0, sizeof(message));
    memset(buffer, 0, sizeof(buffer));

    while (1)
    {
        memset(message, 0, sizeof(message));
        memset(buffer, 0, sizeof(buffer));
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = 0; // removing the \n if exists
        char *msg_tmp = message;
        for (int i = 0; i < 4; i++)
        {
            *msg_tmp = toupper(*msg_tmp);
            msg_tmp++;
        }
        strncpy(temp, message, 4);
        temp[4] = 0;
        if (strcmp("STOR", temp) == 0)
        {
            char temp_message[BUFFER_SIZE] = {0};
            strcpy(temp_message, message);
            char *file_name = strtok(temp_message, " ");
            file_name = strtok(NULL, " ");
            file_name = strtok(NULL, " ");
            if (file_name == NULL)
            {
                printf("err: not enough arguments, please specify file name\n");
                memset(message, 0, sizeof(message));
                memset(buffer, 0, sizeof(buffer));
                continue;
            }

            if (is_file_present(file_name))
            {
                strcpy(stor_state.file_name, file_name);
                send(sock, message, strlen(message), 0);
                read(sock, buffer, 1024);
                sent_stor = true;
            }
            else
            {
                printf("err: specified file does not exist\n");
                continue;
            }
        }
        else if (strcmp("RETR", temp) == 0)
        {
            char temp_message[BUFFER_SIZE] = {0};
            strcpy(temp_message, message);
            char *file_name = strtok(temp_message, " ");
            file_name = strtok(NULL, " ");
            if (file_name == NULL)
            {
                printf("err: not enough arguments, please specify file name\n");
                memset(message, 0, sizeof(message));
                memset(buffer, 0, sizeof(buffer));
                continue;
            }
            strcpy(retr_file_name, file_name);

            send(sock, message, strlen(message), 0);
            read(sock, buffer, 1024);
            printf("%s\n", buffer); // printing buffer here

            // making temp_buffer as strncpy was altering its argument
            char temp_buffer[BUFFER_SIZE] = {0};
            strcpy(temp_buffer, buffer);
            char is_fine[2] = {0};
            strncpy(is_fine, temp_buffer, 1);
            is_fine[1] = 0;

            if (strcmp("-", is_fine) == 0)
            {
                continue;
            }
            else
            {
                retr_file_size = atoi(buffer);
                continue;
            }
        }
        else if (strcmp("SEND", temp) == 0)
        {
            send(sock, message, strlen(message), 0);
            stor_file(sock, retr_file_size, retr_file_name);
            retr_file_size = 0;
            strcpy(retr_file_name, "");
            continue;
        }
        else if (strcmp("SIZE", temp) == 0)
        {
            if (sent_stor)
            {
                sent_stor = false;
                char temp_message[BUFFER_SIZE] = {0};
                strcpy(temp_message, message);
                char *number_of_bytes_str = strtok(temp_message, " ");
                number_of_bytes_str = strtok(NULL, " ");
                if (number_of_bytes_str == NULL)
                {
                    printf("err: not enough arguments, please specify number of bytes\n");
                    memset(message, 0, sizeof(message));
                    memset(buffer, 0, sizeof(buffer));
                    continue;
                }
                int num_of_bytes = atoi(number_of_bytes_str);
                send(sock, message, strlen(message), 0);
                read(sock, buffer, 1024);

                printf("%s\n", buffer); // printing buffer here

                // making temp_buffer as strncpy was altering its argument
                char temp_buffer[BUFFER_SIZE] = {0};
                strcpy(temp_buffer, buffer);
                char is_fine[2] = {0};
                strncpy(is_fine, temp_buffer, 1);
                is_fine[1] = 0;

                if (strcmp("+", is_fine) == 0)
                {
                    char file_addr[BUFFER_SIZE] = {0};
                    sprintf(file_addr, "transfer_files/%s", stor_state.file_name);
                    fp = fopen(file_addr, "r");
                    if (fp == NULL)
                    {
                        printf("err: problem with filename");
                        continue;
                    }
                    send_file(fp, sock, num_of_bytes);
                    memset(buffer, 0, sizeof(buffer));
                    read(sock, buffer, 1024);
                }
                else
                {
                    continue;
                }
            }
            else
            {
                printf("Please send STOR first");
                continue;
            }
        }
        else
        {
            send(sock, message, strlen(message), 0);
            read(sock, buffer, 1024);
        }

        memset(temp, 0, BUFFER_SIZE);
        strncpy(temp, message, 4);
        temp[4] = 0;

        // closes the connection between client and server and closes the client program
        if (strcmp("DONE", temp) == 0 && strcmp("+", buffer) == 0)
        {
            if (close(sock) < 0)
            {
                perror("DONE command failed");
                exit(1);
            }

            return 0;
        }

        printf("%s\n", buffer);
    }
    return 0;
}
