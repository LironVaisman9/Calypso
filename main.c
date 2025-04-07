#include <stdio.h>
#include "fileManager.h"

int main(int argc,char** argv)
{
    char path[] = "app.ae";
    if (fileExist(path))
    {
        printf("yes\n");
    }
    else
    {
        printf("no\n");
    }
    return 0;
}