#include <stdio.h>
#include "fileManager.h"
#include "stringFunctions.h"
#include "steganography.h"

#include <stdlib.h>
#include <string.h>

void printData(char* data, int size) 
{
    for (int i = 0; i < size; i++)
    {
        printf("%c", data[i]);
    }
    printf("\n");
}

int main(int argc,char** argv)
{
    char originalData[] = "Liron Vaisman";
    int dataSize = strlen((char*)originalData);
    char path[] = "C:\\Users\\453D~1\\Desktop\\code.png";
    char path2[] = "C:\\Users\\453D~1\\Desktop\\ds.png";
    char path3[] = "C:\\Users\\453D~1\\Desktop\\res";
    FileObject* object = createFileObject(path);
    FileObject* object2 = createFileObject(path2);
    //encodeFile(object,object2);
    decodeFile(object,path3);
    //encodeMessage(object,originalData);
    //char* decodedData = (char*)malloc(dataSize);
    //decodeMessage(object,&decodedData);
    //printf("Decoded Data: \n");
    //printData(decodedData, dataSize);
    return 0;
}
