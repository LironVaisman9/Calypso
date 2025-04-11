#include <stdio.h>
#include "fileManager.h"
#include "stringFunctions.h"

int main(int argc,char** argv)
{
    char input[10] = {0};
    getUserInput(input,10,NULL);
    printf("%s\n",input);
    return 0;
}