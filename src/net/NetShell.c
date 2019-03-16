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
#include "loc_file.h"
#include "netshell_cmd.h"

int main(int argc, char *argv[]) {
    char message[MAX_MSG_SIZE], buffer[MAX_MSG_SIZE], *args[MAX_CMD_NUM];
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
    int status = 0;
    while (1) {
        printf("NetShell> ");
        fgets(message, MAX_MSG_SIZE, stdin);
        send(cli_socket_fd, message, strlen(message)+1, 0);
        status = parse_cmd(message, args);
        if (status == CLOSE_SERVER || status == EXIT_SHELL) 
            break;
        if (status == GET_FILE) { // TODO
            if (parse_file_format(args[1]) == TXT_FILE)
                recv_txt_file(cli_socket_fd, "../../file/client/recv.txt"); // TODO
            else if (parse_file_format(args[1]) == BIN_FILE)
                recv_bin_file(cli_socket_fd, "../../file/client/recv.jpg"); // TODO
            else
                perror("ERROR : FILE_FORMAT : File's format can't be recognised.\n");
            create_new_connection(&cli_socket_fd, &server_addr);
            continue;
        }
        if (status == PUSH_FILE) { // TODO
            if (parse_file_format(args[1]) == TXT_FILE)
                send_txt_file(cli_socket_fd, "../../file/client/recv.txt"); // TODO
            else if (parse_file_format(args[1]) == BIN_FILE)
                send_bin_file(cli_socket_fd, "../../file/client/recv.jpg"); // TODO
            else
                perror("ERROR : FILE_FORMAT : File's format can't be recognised.\n");
            close(cli_socket_fd);
            create_new_connection(&cli_socket_fd, &server_addr);
            continue;
        }
        recv(cli_socket_fd, buffer, MAX_MSG_SIZE, 0);
        printf("Received message from server : %s\n", buffer);
    }

    close(cli_socket_fd);

    printf("Exit the Netshell.\n");

    return EXIT_SUCCESS;
}
