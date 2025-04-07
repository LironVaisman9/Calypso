#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdbool.h>
#include <stdio.h>

#define READ_BINARY "rb"

typedef struct FileObject
{
    FILE* m_file;
    char* m_path;
    char* m_name;
    char* m_format;
    size_t m_size;
    unsigned char* m_data;
}FileObject;

char* getFileName(char* filePath);
char* getFileFormat(char* filePath);
FileObject* createFileObject(char* filePath);
void freeFileObject(FileObject* fileObject);


bool fileExist(char* filePath);
size_t fileSize(FILE* file);

#endif