

#ifndef HELPER_H_
#define HELPER_H_


/* Functions and defines that are used in some other files. */

/* Asserts given condition and exists in case it is failed. */
#define ASSERT(CONDITION)                                  \
    if (CONDITION) {} else {                               \
        printf("assertion \"%s\" failed\n", #CONDITION);   \
        exit(-1);                                          \
    }

typedef enum bool {false, true} bool;


int parseInt(char *number);
int split(char *input, char splitSymbol, char **result);


#endif

