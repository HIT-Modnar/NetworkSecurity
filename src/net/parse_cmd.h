// Author : Modnar
// Date   : 2019-03-15
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef PARSE_CMD_H
#define PARSE_CMD_H

#define MAX_CMD_LEN 256

#include <stdlib.h>
#include <string.h>

extern int parse_cmd(const char *cmd, char **argv) {
    int argc = 0;
    static char buf_store[MAX_CMD_LEN];
    char *buf = buf_store, *space_pos;
    strcpy(buf, cmd);
    buf[strlen(buf)-1] = ' ';
    while (*buf && (*buf == ' '))
        ++buf;
    space_pos = strchr(buf, ' ');
    while (space_pos) {
        argv[argc++] = buf;
        *space_pos = '\0';
        buf = space_pos + 1;
        while (*buf && (*buf == ' '))
            ++buf;
        space_pos = strchr(buf, ' ');
    }
    return argc;
}

#endif
