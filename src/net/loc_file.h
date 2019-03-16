// Author : Modnar
// Date   : 2019-03-17
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef LOC_FILE_H
#define LOC_FILE_H

#include <stdlib.h>
#include <string.h>

const int TXT_FILE       = 5;
const int BIN_FILE       = 6;
const int FORMAT_ERROR   = -1;

extern int parse_file_format(const char *filename) {
    char *dot_pos = strchr(filename, '.');
    if (!strcmp(dot_pos+1, "txt"))
        return TXT_FILE;
    else if (!strcmp(dot_pos+1, "png"))
        return BIN_FILE;
    else if (!strcmp(dot_pos+1, "jpg"))
        return BIN_FILE;
    return FORMAT_ERROR;
}

#endif
