#include "fileManager.h"
#include "stringFunctions.h"
#include "macros.h"
#include <string.h>
#include <stdlib.h>

bool fileExist(char* filePath)
{
    FILE* file = fopen(filePath,READ_BINARY);
    if (file == NULL)
    {
        return false;
    }
    fclose(file);
    return true;
}
size_t fileSize(FILE* file)
{
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}
char* getFileName(char* filePath)
{
    char* name = NULL;
    int slashPosition = findLast(filePath,'\\') + 1;
    int dotPosition = findLast(filePath,'.');
    if (slashPosition == -1 || dotPosition == -1)
    {
        LOG_ERROR("Could not find '.' or '\\' in the file path");
        return NULL;
    }
    int length = dotPosition - slashPosition;
    name = (char*)malloc(length + 1);
    if (name == NULL)
    {
        LOG_ERROR("Could not allocate memory for the file's name");
        return NULL;
    }
    if (!subString(filePath,name,slashPosition,length))
    {
        LOG_ERROR("Could not create sub string for the file's name");
        free(name);
        return NULL;
    }
    return name;
}
char* getFileFormat(char* filePath)
{
    char* format = NULL;
    int dotPosition = findLast(filePath,'.') + 1;
    if (dotPosition == -1)
    {
        LOG_ERROR("Could not find '.' for the file format");
        return NULL;
    }
    int length = strlen(filePath) - dotPosition;
    if (length > MAX_FORMAT_LENGTH)
    {
        LOG_ERROR("The max length of a file format is: %d",MAX_FORMAT_LENGTH);
        return NULL;
    }
    format = (char*)malloc(length + 1);
    if (format == NULL)
    {
        LOG_ERROR("Could not allocate memory for the file's format");
        return NULL;
    }
    if (!subString(filePath,format,dotPosition,length))
    {
        LOG_ERROR("Could not create sub string for the file's format");
        free(format);
        return NULL;
    }
    return format;
}
FileObject* createFileObject(char* filePath)
{
    if (!fileExist(filePath))
    {
        LOG_ERROR("File does not exist");
        return NULL;
    }
    FileObject* fileObject = NULL;
    fileObject = (FileObject*)malloc(sizeof(FileObject));
    if (fileObject == NULL)
    {
        LOG_ERROR("Could not allocate memory for file object");
        return NULL;
    }
    fileObject->m_file = fopen(filePath,READ_BINARY);
    if (fileObject->m_file == NULL)
    {
        LOG_ERROR("Could not open the file: %s",filePath);
        free(fileObject);
        return NULL;
    }
    fileObject->m_name = getFileName(filePath);
    if (fileObject->m_name == NULL)
    {
        LOG_ERROR("Could not get the file's name");
        fclose(fileObject->m_file);
        free(fileObject);
        return NULL;
    }
    fileObject->m_format = getFileFormat(filePath);
    if (fileObject->m_format == NULL)
    {
        LOG_ERROR("Could not get the file's format");
        fclose(fileObject->m_file);
        free(fileObject->m_name);
        free(fileObject);
        return NULL;
    }
    fileObject->m_size = fileSize(fileObject->m_file);
    fileObject->m_data = (unsigned char*)malloc(fileObject->m_size);
    if (fileObject->m_data == NULL)
    {
        LOG_ERROR("Could not allocate memory for file's binary data");
        fclose(fileObject->m_file);
        free(fileObject->m_name);
        free(fileObject->m_format);
        free(fileObject);
        return NULL;
    }
    fread(fileObject->m_data,fileObject->m_size,1,fileObject->m_file);
    fileObject->m_path = (char*)malloc(strlen(filePath) + 1);
    if (fileObject->m_path == NULL)
    {
        LOG_ERROR("Could not allocate memory for the file's path");
        fclose(fileObject->m_file);
        free(fileObject->m_name);
        free(fileObject->m_format);
        free(fileObject->m_data);
        free(fileObject);
        return NULL;
    }
    strcpy(fileObject->m_path,filePath);
    return fileObject;
}
void freeFileObject(FileObject* fileObject)
{
    free(fileObject->m_data);
    free(fileObject->m_format);
    free(fileObject->m_name);
    free(fileObject->m_path);
    if (fileObject->m_file)
    {
        fclose(fileObject->m_file);
    }
    free(fileObject);
}
void printFileObjct(FileObject* FileObject)
{
    printf("File path: %s\n",FileObject->m_path);
    printf("File name: %s\n",FileObject->m_name);
    printf("File format: %s\n",FileObject->m_format);
    printf("File size: %lu\n",FileObject->m_size);
    //printf("File data:\n");
    //for (size_t i = 0; i < FileObject->m_size; i++)
    //{
        //printf("%02X ", FileObject->m_data[i]);
    //}
    //printf("\n");
}
