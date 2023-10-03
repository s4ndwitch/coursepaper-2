#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>

struct list {
    void *data;
    struct list *nextElement;
};

int getListLength(struct list *list);
struct list *readLines(FILE *file);
void removeList(struct list *list);
char **ltos(struct list *words);
struct list *createList();
void addElement(void *data, struct list *list);
char *compileLine(struct list *list);

#endif