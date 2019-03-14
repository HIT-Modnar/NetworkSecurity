// Author : Modnar
// Date   : 2019-03-14
// Copyright (C) 2019 Modnar. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 256

int main(int argc, char *argv[]) {
    char buffer[MAX_SIZE];
    FILE *fp = fopen("../../file/txt/input.txt", "r");
    fgets(buffer, MAX_SIZE, fp);
    if (fclose(fp)) 
        printf("Error!\n");
    printf("%lu\n", strlen(buffer));
    printf("%s", buffer);

    fp = fopen("../../file/pic/SongHuaBridge.jpg", "rb");
    FILE *wp = fopen("../../file/pic/backup.jpg", "wb");
    if (fp == NULL || wp == NULL) {
        printf("File has problem! Exit the program.\n");
        return EXIT_FAILURE;
    }
    unsigned char buf[MAX_SIZE];
    size_t rc = 0;
    while ((rc = fread(buf, sizeof(unsigned char), MAX_SIZE, fp)) != 0) {
        fwrite(buf, sizeof(unsigned char), rc, wp);
    }
    fclose(fp);
    fclose(wp);
    return EXIT_SUCCESS;
}
