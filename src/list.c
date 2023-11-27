

#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "helper.h"


/* Functions for working with the list itself */

/*
    Creates a struct list and returns its pointer.
*/
struct list *createList() {

    struct list *newList = (struct list *)malloc(sizeof(struct list) * 1);
    newList->data = NULL;
    newList->nextElement = NULL;
    return newList;

}

/*
    Removes given and every next element of a list.
*/
void removeList(struct list *list) {

    struct list *nextElement;
    while (list != NULL) {
        nextElement = list->nextElement;
        free(list);
        list = nextElement;
    }

}

/*
    Adds element to the end of given list.
    Asserts list not being NULL before.
    Fulls up first NULL data (don't mistake
    NULL for equalling zero).
*/
bool addElement(void *data, struct list *list) {

    ASSERT(list != NULL);

    struct list *last_element = NULL;

    do {
        if (list->data == NULL) {
            list->data = data;
            return true;
        }
        last_element = list;
        list = list->nextElement;
    } while (list != NULL);

    struct list *newElement = createList();
    if (newElement == NULL) {
        return false;
    }

    last_element->nextElement = newElement;
    newElement->data = data;

    return true;

}

/*
    Returns the number of elements in given list.
*/
int getListLength(struct list *list) {

    ASSERT(list != NULL);

    int result = 0;
    while (list != NULL) {
        result += 1;
        list = list->nextElement;
    }

    return result;
}

/* Functions for working with a string based on the list */

/*
    Reads given file and returns a list of lines
    presented as lists. 
*/
struct list *readLines(FILE *file) {

    char *symbol = (char *)malloc(sizeof(char) * 1);
    struct list *currentWord = createList();
    struct list *lines = createList();

    ASSERT(currentWord != NULL || lines != NULL);

    while (fscanf(file, "%c", symbol) != EOF) {
        if (*symbol != 10) {
            addElement(symbol, currentWord);
        } else if (*symbol == '\n') {
            *symbol = 0;
            addElement(symbol, currentWord);
            addElement(compileLine(currentWord), lines);
            removeList(currentWord);
            currentWord = createList();
        }
        symbol = (char *)malloc(sizeof(char) * 1);
    }

    addElement(compileLine(currentWord), lines);
    removeList(currentWord);

    return lines;
}

/*
    Transforms given list of strings into an array of strings.
    Removes given list.
*/
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


/*
    Complies a list of chars into a string.
*/
char *compileLine(struct list *list) {
    
    ASSERT(list != NULL);

    char *newWord = (char *)malloc(sizeof(char) * getListLength(list));
    int counter = 0;
    while (list != NULL) {
        *(newWord + counter++) = *((char *)(list->data));
        list = list->nextElement;
    }

    return newWord;
}

