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
    char *test_str = "Test string.", buffer[MAX_DATA_SIZE];
    if (argc > 1)
        test_str = argv[1];
    int cli_socket_fd, number_bytes;
    struct sockaddr_in server_addr;

    cli_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LOC_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(cli_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(cli_socket_fd, test_str, strlen(test_str)+1, 0);
    recv(cli_socket_fd, buffer, MAX_DATA_SIZE, 0);

    strcpy(test_str, "exit\n");
    send(cli_socket_fd, test_str, strlen(test_str)+1, 0);

    close(cli_socket_fd);

    printf("Received from server : %s\n", buffer);

    return EXIT_SUCCESS;
}
