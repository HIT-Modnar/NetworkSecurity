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
#include "parse_cmd.h"

const int MAX_CMD_NUM = 16;

const int RETURN_SUCCESS = 0;
const int EXIT_SHELL     = 1;
const int CLOSE_SERVER   = 2;
const int GET_TXT_FILE   = 3;
const int GET_BIN_FILE   = 4;
const int GET_ERROR      = -1;

/**
 *   In order to maintian the status that user could still send command to server 
 * after get a file from a server, here defines a function to create a new socket 
 * to meet the requirement.
 */
int create_new_connection(int *cli_socket_fd, struct sockaddr_in *server_addr) {
    // Create a new socket_fd to cli_socket_fd.
    *cli_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    // Make the new socket_fd connect to the target server.
    connect(*cli_socket_fd, (struct sockaddr *)server_addr, sizeof(*server_addr));
    return EXIT_SUCCESS;
}

int exec_cmd(const char *cmd) {
    char *argv[MAX_CMD_NUM];
    int argc = parse_cmd(cmd, argv);
    if (argc > 0) {
        if (!strcmp(argv[0], "exit"))
            return EXIT_SHELL;
        if (!strcmp(argv[0], "close") && !strcmp(argv[1], "server"))
            return CLOSE_SERVER;
        if (!strcmp(argv[0], "get")) {
            char *dot_pos = strchr(argv[1], '.');
            if (!strcmp(dot_pos+1, "txt"))
                return GET_TXT_FILE;
            else if (!strcmp(dot_pos+1, "png"))
                return GET_BIN_FILE;
            else if (!strcmp(dot_pos+1, "jpg"))
                return GET_BIN_FILE;
            else 
                return GET_ERROR;
        }
    }
    return RETURN_SUCCESS;
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
    int status = 0;
    while (status != EXIT_SHELL) {
        printf("NetShell> ");
        fgets(message, MAX_MSG_SIZE, stdin);
        send(cli_socket_fd, message, strlen(message)+1, 0);
        status = exec_cmd(message);
        if (status == CLOSE_SERVER) 
            break;
        if (status == GET_TXT_FILE) { // TODO
            recv_txt_file(cli_socket_fd, "../../file/client/recv.txt");
            create_new_connection(&cli_socket_fd, &server_addr);
            continue;
        }
        if (status == GET_BIN_FILE) { // TODO
            recv_bin_file(cli_socket_fd, "../../file/client/recv.jpg");
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
