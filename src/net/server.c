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

#include "loc_net.h"
#include "loc_file.h"
#include "netshell_cmd.h"

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_MSG_SIZE], message[MAX_MSG_SIZE];
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

    char *args[MAX_CMD_NUM];
    while (1) {
        bool exit_server = false;
        printf("\nWaiting for connection ... \n");
        addr_len = sizeof(client_addr);
        connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);

        while (1) {
            int n = recv(connect_fd, buffer, MAX_MSG_SIZE, 0);
            int status = parse_cmd(buffer, args);
            // Parsing the command from user to judge whether need to close server.
            if (status == CLOSE_SERVER) {
                exit_server = true;
                break;
            }
            // Parsing the command to judge whether need to close current connection 
            // and create a new connect_fd.
            if (status == EXIT_SHELL)
                break;
            if (status == GET_FILE) {
                if (parse_file_format(args[1]) == TXT_FILE)
                    send_txt_file(connect_fd, "../../file/server/send.txt"); // TODO
                else if (parse_file_format(args[1]) == BIN_FILE)
                    send_bin_file(connect_fd, "../../file/server/pic.jpg"); // TODO
                else 
                    perror("ERROR : SERVER : File's format can't be recognised\n");
                break;
            }
            if (status == PUSH_FILE) {
                if (parse_file_format(args[1]) == TXT_FILE)
                    recv_txt_file(connect_fd, "../../file/server/recv.txt"); // TODO
                else if (parse_file_format(args[1]) == BIN_FILE)
                    recv_bin_file(connect_fd, "../../file/server/recv.jpg"); // TODO
                else 
                    perror("ERROR : SERVER : File's format can't be recognised\n");
                break;
            }

            printf("Received from %s : %d \n", inet_ntop(AF_INET, &client_addr, 
                    message, sizeof(message)), ntohs(client_addr.sin_port));

            for (int i = 0; i < n; ++i) 
                buffer[i] = toupper(buffer[i]);

            send(connect_fd, buffer, n+1, 0);
            printf("Send : %s\n", buffer);
        }
        close(connect_fd);
        if (exit_server)
            break;
    }
    printf("Closing the Server...\n");
    close(listen_fd);
    printf("Done.\n");
    return EXIT_SUCCESS;
}
