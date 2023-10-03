#include <string.h>
#include <stdlib.h>

#include "helper.h"
#include "list.h"

int parseInt(char *number) {
	int result = 0;
	int digit = 1;
	for (int i = strlen(number) - 1; i > -1; i--) {
		switch (number[i]) {
			case '0':
				result += 0 * digit;
				break;
			case '1':
				result += 1 * digit;
				break;
			case '2':
				result += 2 * digit;
				break;
			case '3':
				result += 3 * digit;
				break;
			case '4':
				result += 4 * digit;
				break;
			case '5':
				result += 5 * digit;
				break;
			case '6':
				result += 6 * digit;
				break;
			case '7':
				result += 7 * digit;
				break;
			case '8':
				result += 8 * digit;
				break;
			case '9':
				result += 9 * digit;
				break;
			default:
				return -1;
		}
		digit *= 10;
	}
	return result;
}

int split(char *input, char splitSymbol, char **result) {

    char *symbol = (char *)malloc(sizeof(char) * 1);
    struct list *currentWord = createList();
    struct list *words = createList();
    if (currentWord == NULL || words == NULL) {
        fprintf(stderr, "An error occured while creating a list\n");
        return -1;
    }

    for (int i = 0; i < strlen(input); i++) {
        *symbol = input[i];
        if (*symbol != splitSymbol) {
            addElement(symbol, currentWord);
        } else {
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
    int length = getListLength(words);

    char **temp = ltos(words);
    for (int i = 0; i < length; i++) {
        result[i] = temp[i];
    }

    return length;
}