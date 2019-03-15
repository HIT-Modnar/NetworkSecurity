// Author : Modnar
// Date   : 2019-03-14
// Copyright (C) 2019 Modnar. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// As for the local port, should choose from 1025 to 65535. 
// Because the ports no more than 1024 are remained by operating system. 
const int LOC_PORT = 8888;
const int MAX_DATA_SIZE = 256;
const int BUFFER_SIZE = 1024;

int recv_txt_file(int cli_socket_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        perror("File : not found.\n");
        return EXIT_FAILURE;
    } else {
        bzero(buffer, BUFFER_SIZE);
        int length = 0;
        while ((length = recv(cli_socket_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            if (fwrite(buffer, sizeof(char), length, fp) < length) {
                perror("File : write failed.\n");
                return EXIT_FAILURE;
            }
            bzero(buffer, BUFFER_SIZE);
        }
    }
    fclose(fp);
    printf("Transmission finished.\n");
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    char message[MAX_DATA_SIZE], buffer[MAX_DATA_SIZE];
    int cli_socket_fd, number_bytes;
    struct sockaddr_in server_addr;

    cli_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LOC_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(cli_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Because the fgets function will read the '\n', the analyzing condition should
    // compare message with "exit\n".
    while (strcmp(message, "exit\n")) {
        printf("NetShell> ");
        fgets(message, MAX_DATA_SIZE, stdin);
        send(cli_socket_fd, message, strlen(message)+1, 0);
        if (strcmp(message, "close server\n") == 0)
            break;
        if (strcmp(message, "send.txt\n") == 0) { // TODO
            recv_txt_file(cli_socket_fd, "../../file/client/recv.txt");
            continue;
        }
        recv(cli_socket_fd, buffer, MAX_DATA_SIZE, 0);
        printf("Received from server : %s\n", buffer);
    }

    close(cli_socket_fd);

//    printf("Received from server : %s\n", buffer);
    printf("Exit the Netshell.\n");

    return EXIT_SUCCESS;
}
