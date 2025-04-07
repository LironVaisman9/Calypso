#include <stdio.h>
#include "fileManager.h"

int main(int argc,char** argv)
{
    char path[] = "C:\\Users\\453D~1\\Desktop\\2024-03-01 23-07-48.mkv";
    FileObject* object = createFileObject(path);
    printFileObjct(object);
    return 0;
}