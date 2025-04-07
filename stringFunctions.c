#include "stringFunctions.h"
#include <string.h>
#include <stdlib.h>
#include "macros.h"

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