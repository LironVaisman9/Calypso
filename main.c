#include <stdio.h>
#include "fileManager.h"
#include "stringFunctions.h"
#include "steganography.h"

#include <stdlib.h>
#include <string.h>

void printData(unsigned char* data, int size) 
{
    for (int i = 0; i < size; i++)
    {
        printf("%c", data[i]);
    }
    printf("\n");
}

int main(int argc,char** argv)
{
    unsigned char originalData[] = "Testing 03!@@ tesit test testing 5092!@)\\321";
    int dataSize = strlen((char*)originalData);
    char path[] = "C:\\Users\\453D~1\\Desktop\\github.png";
    FileObject* object = createFileObject(path);
    encode(object->m_data,originalData,150,dataSize);
    unsigned char* decodedData = (unsigned char*)malloc(dataSize);
    decode(object->m_data,decodedData,150,dataSize);
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
