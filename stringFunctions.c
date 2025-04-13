#include "stringFunctions.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "macros.h"

/// @brief gets user input in a range of maximum length
/// @param input char* that will hold the user input
/// @param maxLength the maximum length of the input
/// @param text text that will be printed before taking the input
/// @return true if the process was successful and false if no
bool getUserInput(char* input,size_t maxLength,const char* text)
{
    if (text != NULL)
    {
        printf("%s",text);
    }
    char* temp = (char*)malloc((sizeof(char) * maxLength) + 1);
    if (temp == NULL)
    {
        LOG_ERROR("Could not allocate memory for temp string");
        return false;
    }
    fgets(temp,maxLength + 1,stdin);
    if (strchr(temp, '\n') == NULL)
    {
        LOG_ERROR("Maximum input length is: %zu",maxLength);
        free(temp);
        return false;
    }
    temp[strcspn(temp,"\n")] = '\0';
    strcpy(input,temp);
    free(temp);
    return true;
}
/// @brief creates a sub string from a index to index+length
/// @param str the original string
/// @param subStr where the sub string will be saved
/// @param index the starting index of the sub string
/// @param legnth the length of the substring
/// @return true if the process was successful and false if no
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
/// @brief finds last occurrence of a char in a string
/// @param str the string
/// @param ch the char
/// @return the index of the last occurrence(-1 if the char was not found)
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
/// @brief creates a random string in a spesific length
/// @param str where the string will be saved
/// @param length the length of the random string
/// @param seed the randomness seed
/// @param nullTerminate should there be a '\0' or no
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
/// @brief converts string to binary data
/// @param str the string 
/// @param length the length of the string
/// @return the binary data
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
/// @brief converts binary data to string
/// @param binary the binary data
/// @param length the length of the binary data 
/// @return the string
char* binaryToString(unsigned char* binary,size_t length)
{
    char* str = NULL;
    if (binary == NULL || length == 0)
    {
        LOG_ERROR("Null binary data or zero length");
        return NULL;
    }
    str = (char*)malloc(length + 1);  
    if (str == NULL)
    {
        LOG_ERROR("Could not allocate memory for the str");
        return NULL;
    }
    memcpy(str, binary, length);
    str[length] = '\0'; 
    return str;
}
/// @brief combines two strings to create a full file-path
/// @param str1 the first string
/// @param str2 the second string
/// @return the combined full path
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