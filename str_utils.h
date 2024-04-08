#ifndef STR_UTILS_H
#define STR_UTILS_H

// rtrim() and trim() modify the string in-place
char *ltrim(char *str);
char *rtrim(char *str);
char *trim(char *str);

#endif // end STR_UTILS_H

#ifdef STR_UTILS_IMPL

#include <string.h>
#include <ctype.h>

char *ltrim(char *str) {
    while (isspace(*str))
        ++str;
    return str;
}

char *rtrim(char *str) {
    char *p = str + strlen(str) - 1;
    while (isspace(*p) && p > str)
        --p;
    *(p + 1) = '\0';
    return str;
}

char *trim(char *str) {
    return rtrim(ltrim(str));
}

#endif // end implementation
