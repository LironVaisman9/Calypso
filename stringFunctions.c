#include "stringFunctions.h"
#include <string.h>
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