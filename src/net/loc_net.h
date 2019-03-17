// Author : Modnar
// Date   : 2019-03-15
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef LOC_NET_H
#define LOC_NET_H

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>


// As for the local port, should choose from 1024 to 65535. 
// Because the ports no more than 1023 are remained by operating system. 
#define LOC_PORT 8888
#define MAX_MSG_SIZE 256
#define BUFFER_SIZE 1024

/**
 *   In order to maintian the status that user could still send command to server 
 * after get a file from a server, here defines a function to create a new socket 
 * to meet the requirement.
 *
 * @param *cli_socket_fd The pointer to client's scoket_fd.
 * @param *server_addr The pointer to server's Internet socket address struct object.
 * @return EXIT_SUCCESS if create successfully, otherwise nothing.
 */
int create_new_connection(int *cli_socket_fd, struct sockaddr_in *server_addr) {
    // Create a new socket_fd to cli_socket_fd.
    *cli_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    // Make the new socket_fd connect to the target server.
    connect(*cli_socket_fd, (struct sockaddr *)server_addr, sizeof(*server_addr));
    return EXIT_SUCCESS;
}

/**
 * Send a txt file from server (or client) to client (or server).
 *
 * @param connect_fd The connection's socket_fd.
 * @param file_path The file path of the txt file which prepared to be sent.
 * @return EXIT_SUCCESS if send successfully, otherwise nothing.
 */
extern int send_txt_file(int connect_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL)
        perror("ERROR : SEND_TXT_FILE : Source file not found!\n");
    else {
        bzero(buffer, BUFFER_SIZE);
        int length = 0;
        while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
            if (send(connect_fd, buffer, length, 0) < 0) {
                perror("ERROR : SEND_TXT_FILE : Send source file failed.\n");
                break;
            }
            bzero(buffer, BUFFER_SIZE);
        }
    }
    fclose(fp);
    printf("Transmission finished.\n");
    return EXIT_SUCCESS;
}

/**
 * Receive a txt file from server (or client).
 *
 * @param cli_socket_fd The connection's socket_fd.
 * @param file_path The file path of local txt file which prepared to receive.
 * @return EXIT_SUCCESS if receive successfully, otherwise nothing.
 */
extern int recv_txt_file(int cli_socket_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        perror("ERROR : RECV_TXT_FILE : Target file not found.\n");
        return EXIT_FAILURE;
    } else {
        bzero(buffer, BUFFER_SIZE);
        int length = 0;
        while ((length = recv(cli_socket_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            if (fwrite(buffer, sizeof(char), length, fp) < length) {
                perror("ERROR : RECV_TXT_FILE : Write target file failed.\n");
                return EXIT_FAILURE;
            }
            bzero(buffer, BUFFER_SIZE);
        }
    }
    fclose(fp);
    printf("Transmission finished.\n");
    return EXIT_SUCCESS;
}

/**
 * Send a binary file from server (or client) to client (or server).
 *
 * @param connect_fd The connection's socket_fd.
 * @param file_path The file path of the binary file which prepared to be sent.
 * @return EXIT_SUCCESS if send successfully, otherwise nothing.
 */
extern int send_bin_file(int connect_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL)
        perror("ERROR : File : Not found!\n");
    else {
        bzero(buffer, BUFFER_SIZE);
        int length = 0;
        while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
            if (send(connect_fd, buffer, length, 0) < 0) {
                perror("ERROR : File : Send failed.\n");
                break;
            }
            bzero(buffer, BUFFER_SIZE);
        }
    }
    fclose(fp);
    printf("Transmission finished.\n");
    return EXIT_SUCCESS;
}

/**
 * Receive a binary file from server (or client).
 *
 * @param cli_socket_fd The connection's socket_fd.
 * @param file_path The file path of local binary file which prepared to receive.
 * @return EXIT_SUCCESS if recevie successfully, otherwise nothing.
 */
extern int recv_bin_file(int cli_socket_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "wb");
    if (fp == NULL) {
        perror("ERROR : File : Not found.\n");
        return EXIT_FAILURE;
    } else {
        bzero(buffer, BUFFER_SIZE);
        int length = 0;
        while ((length = recv(cli_socket_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            if (fwrite(buffer, sizeof(char), length, fp) < length) {
                perror("ERROR : File : Write failed.\n");
                return EXIT_FAILURE;
            }
            bzero(buffer, BUFFER_SIZE);
        }
    }
    fclose(fp);
    printf("Transmission finished.\n");
    return EXIT_SUCCESS;
}

#endif /* LOC_NET_H */
