#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdbool.h>
#include <stdio.h>

#define MAX_FORMAT_LENGTH 4
#define READ_BINARY "rb"
#define WRITE_BINARY "wb"
#define READ_WRITE_BINARY "w+b"

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
void printFileObjct(FileObject* FileObject);


bool fileExist(char* filePath);
size_t fileSize(FILE* file);

#endif