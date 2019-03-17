// Author : Modnar
// Date   : 2019-03-15
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef NETSHELL_CMD_H
#define NETSHELL_CMD_H

const int MAX_CMD_NUM    = 16;
const int MAX_CMD_LEN    = 256;

const int PARSE_SUCCESS  = 0;
const int EXIT_SHELL     = 1;
const int CLOSE_SERVER   = 2;
const int GET_FILE       = 3;
const int PUSH_FILE      = 4;

#include <stdlib.h>
#include <string.h>

/**
 * Format the command. 
 *   The function can delete the useless SPACE character.
 * 
 * @param cmd The command which needed to be formatted.
 * @param argv The argument's value (const char *) array.
 * @return The command argument's count.
 */
extern int format_cmd(const char *cmd, char **argv) {
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

/** 
 * Parse a command.
 *   Judge whether the usage of the command is right.
 *
 * @param cmd The command string which need to be parse.
 * @param argv The arguments' array. And after parsing, the array is also formatted. 
 * @return PARSE_SUCCESS if parse successfully, or some other status code after 
 *         parsing successfully (e.g. GET_FILE, PUSH_FILE).
 */
extern int parse_cmd(const char *cmd, char *argv[]) {
    int argc = format_cmd(cmd, argv);
    if (argc > 0) {
        if (!strcmp(argv[0], "exit"))
            return EXIT_SHELL;
        if (!strcmp(argv[0], "close") && !strcmp(argv[1], "server"))
            return CLOSE_SERVER;
        if (!strcmp(argv[0], "get")) {
            if (argc != 2)
                perror("Usage : get filename\n");
            else 
                return GET_FILE;
        }
        if (!strcmp(argv[0], "push")) {
            if (argc != 2)
                perror("Usage : push filename\n");
            else 
                return PUSH_FILE;
        }
    }
    return PARSE_SUCCESS;
}

#endif
