#ifndef STRINGFUNCTIONS_H
#define STRINGFUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>

void getUserInput(char* input,size_t maxLength,const char* text);
bool subString(char* str,char* subStr,int index,int legnth);
int findLast(char* str,char ch);

#endif