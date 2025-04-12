#include "stringFunctions.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "macros.h"

void getUserInput(char* input,size_t maxLength,const char* text)
{
    if (text != NULL)
    {
        printf("%s",text);
    }
    char* temp = (char*)malloc((sizeof(char) * maxLength) + 1);
    if (temp == NULL)
    {
        LOG_ERROR("Could not allocate memory for temp string");
        return;
    }
    fgets(temp,maxLength + 1,stdin);
    if (strchr(temp, '\n') == NULL)
    {
        LOG_ERROR("Maximum input length is: %zu",maxLength);
        free(temp);
        return;
    }
    temp[strcspn(temp,"\n")] = '\0';
    strcpy(input,temp);
    free(temp);
}

bool subString(char* str,char* subStr,int index,int legnth)
{
    if ((index + legnth) > strlen(str))
    {
        LOG_ERROR("Substring is out of rage");
        return false;
    }
    if (index >= strlen(str))
    {
        LOG_ERROR("Index is out of range");
        return false;
    }
    int i = 0;
    while (i < legnth && str[index + i] != '\0')
    {
        subStr[i] = str[index + i];
        i++;
    }
    subStr[legnth] = '\0';
    return true;
}
int findLast(char* str,char ch)
{
    size_t length = strlen(str);
    int i = 0;
    int position = -1;
    for (i = 0; i < length; i++)
    {
        if (str[i] == ch)
        {
            position = i;
        }
    }
    return position;
}

void randomString(char* str, size_t length,unsigned int seed, bool nullTerminate)
{
    const char charSet[] = ALPHBET;
    size_t charSetSize = sizeof(charSet) - 1;
    size_t i = 0;
    if (str == NULL || length == 0)
    {
        LOG_ERROR("Str or length are empty");
        return;
    }
    srand(seed);
    for (i = 0; i < length; i++)
    {
        str[i] = charSet[rand() % charSetSize];
    }
    if (nullTerminate && length > 0) 
    {
        str[length - 1] = '\0'; 
    }
    srand(time(NULL));
}
unsigned char* stringToBinary(char* str,size_t length)
{
    if (str == NULL || length == 0)
    {
        LOG_ERROR("Null string or zero length");
        return NULL;
    }
    unsigned char* bytes = NULL;
    bytes = (unsigned char*)malloc(length);
    if (bytes == NULL)
    {
        LOG_ERROR("Could not allocate memory for bytes of: %s",str);
        return NULL;
    }
    memcpy(bytes, str,length);
    return bytes;
}
char* binaryToString(unsigned char* binary,size_t length)
{
    char* str = NULL;
    if (binary == NULL || length == 0)
    {
        LOG_ERROR("Null binary data or zero length");
        return NULL;
    }
    str = (char*)malloc(length);
    if (str == NULL)
    {
        LOG_ERROR("Could not allocate memory for the str");
        return NULL;
    }
    memcpy(str, binary, length);
    return str;
}
char* combinePath(char* str1,char* str2)
{
    char* fullPath = NULL;
    if (str1 == NULL || str2 == NULL)
    {
        LOG_ERROR("The strings must not be NULL");
        return NULL;
    }
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t totalLength = len1 + len2 + 2;
    fullPath = (char*)malloc(totalLength);
    if (fullPath == NULL)
    {
        LOG_ERROR("Could not allocate memory for full path");
        return NULL;
    }
    snprintf(fullPath, totalLength, "%s.%s", str1, str2);
    return fullPath;
}