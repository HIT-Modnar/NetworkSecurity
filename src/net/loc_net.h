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

extern int send_txt_file(int connect_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "r");
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

extern int recv_txt_file(int cli_socket_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "w");
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

extern int send_bin_file(int connect_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "rb");
    printf("Run at here.\n");
    if (fp == NULL)
        perror("ERROR : File : Not found!\n");
    else {
        printf("Enter right way\n");
        bzero(buffer, BUFFER_SIZE);
        int length = 0;
        while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
            printf("Enter loop way\n");
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

extern int recv_bin_file(int cli_socket_fd, const char *file_path) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file_path, "wb");
    if (fp == NULL) {
        perror("ERROR : File : Not found.\n");
        return EXIT_FAILURE;
    } else {
        printf("Run in the right way.\n");
        bzero(buffer, BUFFER_SIZE);
        int length = 0;
        while ((length = recv(cli_socket_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            printf("Into the loop way.\n");
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

#endif
