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
const int FILE_EXIST     = 0;
const int FILE_NOT_EXIST = -2;

/**
 * Parse a file's format.
 *
 * @param filename The file's name or path which need to parse its format.
 * @return The file's category status code if parse successfully, otherwise the 
 *         FORMAT_ERROR status code.
 */
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

/**
 * Judge whether the source file is exist.
 *
 * @param filename The file's name or path needed to be searched.
 * @return FILE_EXIST if have found the source file, otherwise the FILE_NOT_EXIST
 *         status code.
 */
// TODO
extern int find_file(const char *filename);

#endif /* LOC_FILE_H */
