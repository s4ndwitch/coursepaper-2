#include <stdlib.h>
#include <stdio.h>

#include "list.h"

struct list *createList() {
    struct list *newList = (struct list *)malloc(sizeof(struct list) * 1);
    newList->data = NULL;
    newList->nextElement = NULL;
}

int getListLength(struct list *list) {
    if (list == NULL) {
        return -1;
    }

    int result = 0;
    while (list != NULL) {
        result += 1;
        list = list->nextElement;
    }

    return result;
}

void addElement(void *data, struct list *list) {

    if (list == NULL) {
        list = createList();
    }

    while (list->nextElement != 0x0) {
        if (list->data == 0x0) {
            list->data = data;
            return;
        }
        list = list->nextElement;
    }

    if (list->data == 0x0) {
        list->data = data;
        return;
    }

    struct list *newElement = createList();
    if (newElement == NULL) {
        return;
    }

    list->nextElement = newElement;
    newElement->data = data;

}

char *compileLine(struct list *list) {
    if (list == NULL) {
        return NULL;
    }

    char *newWord = (char *)malloc(sizeof(char) * getListLength(list));
    int counter = 0;
    while (list != NULL) {
        *(newWord + counter++) = *((char *)(list->data));
        list = list->nextElement;
    }

    return newWord;
}

void removeList(struct list *list) {
    struct list *nextElement;
    while (list != NULL) {
        nextElement = list->nextElement;
        free(list);
        list = nextElement;
    }
}

struct list *readLines(FILE *file) {

    char *symbol = (char *)malloc(sizeof(char) * 1);
    struct list *currentWord = createList();
    struct list *words = createList();
    if (currentWord == NULL || words == NULL) {
        fprintf(stderr, "An error occured while creating a list\n");
        return NULL;
    }

    while (fscanf(file, "%c", symbol) != EOF) {
        if (*symbol != 10) {
            addElement(symbol, currentWord);
        } else if (*symbol == 10) {
            free(symbol);
            symbol = (char *)malloc(sizeof(char) * 1);
            *symbol = 0;
            addElement(symbol, currentWord);
            addElement(compileLine(currentWord), words);
            removeList(currentWord);
            currentWord = createList();
        }
        symbol = (char *)malloc(sizeof(char) * 1);
    }

    addElement(compileLine(currentWord), words);
    removeList(currentWord);

    return words;
}

char **ltos(struct list *words) {
    char **result = (char **)malloc(sizeof(char *) * getListLength(words));
    struct list *temp = words;
    int counter = 0;

    while (temp != NULL) {
        result[counter++] = temp->data;
        temp = temp->nextElement;
    }

    removeList(words);

    return result;
}
