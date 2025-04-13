#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdbool.h>
#include <stdio.h>

#define MAX_FORMAT_LENGTH 4
#define READ_BINARY "rb"
#define WRITE_BINARY "wb"
#define APPEND_BINARY "ab"

typedef struct FileObject
{
    char* m_path;
    char* m_name;
    char* m_format;
    size_t m_size;
    unsigned char* m_data;
    int m_width;
    int m_height;
    int m_channels;
}FileObject;

char* getFileName(char* filePath);
char* getFileFormat(char* filePath);
FileObject* createFileObject(char* filePath);
void freeFileObject(FileObject* fileObject);
void printFileObjct(FileObject* FileObject);


bool fileExist(char* filePath);
size_t fileSize(FILE* file);

#endif