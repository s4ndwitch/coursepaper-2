

#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include "helper.h"

/*
    The list is presented as a horizontal structure.
    I. e. an variable of struct list might be any
    part of a list: a head, a tail or an inner element.
    They are equal.
*/

struct list {
    void *data;
    struct list *nextElement;
};

/* Functions for working with the list itself */

struct list *createList();
void removeList(struct list *list);

bool addElement(void *data, struct list *list);
bool removeElement(void *data, struct list *list);
int getListLength(struct list *list);

/* Functions for working with a string based on the list */

struct list *readLines(FILE *file);
char **ltos(struct list *words);
char *compileLine(struct list *list);


#endif

