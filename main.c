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
    char originalData[] = "Testing test yes tes!!!0932 s3";
    int dataSize = strlen((char*)originalData);
    char path[] = "C:\\Users\\453D~1\\Desktop\\code.png";
    FileObject* object = createFileObject(path);
    encodeMessage(object,originalData);
    char* decodedData = (char*)malloc(dataSize);
    decodeMessage(object,&decodedData);
    printf("Original Data: \n");
    printData(originalData, dataSize);
    printf("Decoded Data: \n");
    printData(decodedData, dataSize);
    // Check if the original and decoded data are the same
    if (memcmp(originalData, decodedData, dataSize) == 0) {
        printf("Success! The decoded data matches the original data.\n");
    } else {
        printf("Error! The decoded data does not match the original data.\n");
    }
    printf("magic: %s",createMagic(object));
    return 0;
}
