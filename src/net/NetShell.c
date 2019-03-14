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

// struct sockaddr_in {
//    unsigned short int sin_family;
//    uint16_t sin_port;
//    struct in_addr sin_addr;
//    unsigned char sin_zero[8];
// };
// 
// struct in_addr {
//     uint32_t s_addr;
// };

// Format the IP address from dot split's to long type(have changed to net formation).
// uint32_t inet_addr("xxx.xxx.xxx.xxx");

int main(int argc, char *argv[]) {
    char message[MAX_DATA_SIZE], buffer[MAX_DATA_SIZE];
    if (argc > 1)
        strcpy(message, argv[1]);
    int cli_socket_fd, number_bytes;
    struct sockaddr_in server_addr;

    cli_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LOC_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(cli_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (strcmp(message, "exit")) {
        printf("NetShell> ");
        fgets(message, MAX_DATA_SIZE, stdin);
        send(cli_socket_fd, message, strlen(message)+1, 0);
        recv(cli_socket_fd, buffer, MAX_DATA_SIZE, 0);
        printf("Received from server : %s\n", buffer);
    }

    close(cli_socket_fd);

//    printf("Received from server : %s\n", buffer);
    printf("Exit the Netshell.\n");

    return EXIT_SUCCESS;
}
