#include "revert_string.h"

void RevertString(char *str) {
    char *end = str;

    while (*end != '\0') {
        end++;
    }
    end--;

    while (str < end) {
        char temp = *str;
        *str = *end;
        *end = temp;
        str++;
        end--;
    }
}
