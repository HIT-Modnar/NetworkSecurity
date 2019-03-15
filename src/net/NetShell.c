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

#include "loc_net.h"

int create_new_connection(int *cli_socket_fd, struct sockaddr_in *server_addr) {
    // Create a new socket_fd to cli_socket_fd.
    *cli_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    // Make the new socket_fd connect to the target server.
    connect(*cli_socket_fd, (struct sockaddr *)server_addr, sizeof(*server_addr));
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    char message[MAX_MSG_SIZE], buffer[MAX_MSG_SIZE];
    int cli_socket_fd, number_bytes;
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    // Initilize the target server's information.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LOC_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    create_new_connection(&cli_socket_fd, &server_addr);

    // Because the fgets function will read the '\n', the analyzing condition should
    // compare message with "exit\n".
    while (strcmp(message, "exit\n")) {
        printf("NetShell> ");
        fgets(message, MAX_MSG_SIZE, stdin);
        send(cli_socket_fd, message, strlen(message)+1, 0);
        if (strcmp(message, "close server\n") == 0)
            break;
        if (strcmp(message, "send.txt\n") == 0) { // TODO
            recv_txt_file(cli_socket_fd, "../../file/client/recv.txt");
            create_new_connection(&cli_socket_fd, &server_addr);
            continue;
        }
        recv(cli_socket_fd, buffer, MAX_MSG_SIZE, 0);
        printf("Received from server : %s\n", buffer);
    }

    close(cli_socket_fd);

    printf("Exit the Netshell.\n");

    return EXIT_SUCCESS;
}
