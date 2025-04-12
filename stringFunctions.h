#ifndef STRINGFUNCTIONS_H
#define STRINGFUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>

#define ALPHBET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

void getUserInput(char* input,size_t maxLength,const char* text);
bool subString(char* str,char* subStr,int index,int legnth);
int findLast(char* str,char ch);
void randomString(char* str, size_t length,unsigned int seed, bool nullTerminate);
unsigned char* stringToBinary(char* str,size_t length);
char* binaryToString(unsigned char* binary,size_t length);

#endif