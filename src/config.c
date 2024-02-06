


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "list.h"
#include "helper.h"
#include "config.h"


/*
    Reads a config file by given name and returns
    an array of ints: the height and width of a cell,
    number of cells in a row and a column.
*/
int *readConfig(char *fileName) {

    FILE *configFile = fopen(fileName, "r");
    if (configFile == NULL) {
        fprintf(stderr, "Could not open config file\n");
        return NULL;
    }

    int height = -1, width = -1, x = -1, y = -1;

    struct list *lines = readLines(configFile);
    int length = getListLength(lines);
    char **strLines = ltos(lines);

    for (int i = 0; i < length; i++) {
        char **words = (char **)malloc(sizeof(char *) * length);
        int lineLength = split(strLines[i], ' ', words);

        if (lineLength < 3) {
            fprintf(stderr, "Not enough elements in a line\n");
            return NULL;
        }
        if (!strcmp(words[0], "height")) {
            height = parseInt(words[2]);
        } else if (!strcmp(words[0], "width")) {
            width = parseInt(words[2]);
        } else if (!strcmp(words[0], "x")) {
            x = parseInt(words[2]);
        } else if (!strcmp(words[0], "y")) {
            y = parseInt(words[2]);
        }
    }

    if (x < 1 || y < 1 || width < 1 || height < 1) {
        fprintf(stderr, "Not enough variables in a config file\n");
        return NULL;
    }
    int *result = (int *)malloc(sizeof(int) * 4);
    result[0] = height;
    result[1] = width;
    result[2] = x;
    result[3] = y;

    fclose(configFile);
    return result;
}

