#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <ctype.h>

// Remove espacos em braco a esquerda
char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

// Remove espacos em braco a direita
char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

// Remove espacos em braco a direita e a esquerda
char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

#endif // UTILS_H
