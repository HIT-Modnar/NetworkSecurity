// Author : Modnar
// Date   : 2019-03-14
// Copyright (C) 2019 Modnar. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

const int LOC_PORT = 8888;
const int MAX_DATA_SIZE = 256;

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_DATA_SIZE], message[MAX_DATA_SIZE];
    int listen_fd, connect_fd;
    unsigned addr_len;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(buffer, 0, sizeof(buffer));
    memset(message, 0, sizeof(message));
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LOC_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listen_fd, (struct sockaddr *)&server_addr, 16);

    listen(listen_fd, 20);

    while (1) {
        bool exit_server = false;
        printf("\nWaiting for connection ... \n");
        addr_len = sizeof(client_addr);
        printf("%d\n", addr_len);
        connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);

        int n = recv(connect_fd, buffer, MAX_DATA_SIZE, 0);
        while (1) {
            if (strcmp(buffer, "close server\n") == 0) {
                exit_server = true;
                break;
            }
            if (strcmp(buffer, "exit\n") == 0)
                break;

            printf("Received from %s : %d \n", inet_ntop(AF_INET, &client_addr, 
                    message, sizeof(message)), ntohs(client_addr.sin_port));

            for (int i = 0; i < n; ++i) 
                buffer[i] = toupper(buffer[i]);

            send(connect_fd, buffer, n+1, 0);
            printf("Send : %s\n", buffer);
            n = recv(connect_fd, buffer, MAX_DATA_SIZE, 0);
        }
        close(connect_fd);
        if (exit_server)
            break;
    }
    printf("Closing the Server...\nDone.\n");
    return EXIT_SUCCESS;
}
