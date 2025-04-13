#include "fileManager.h"
#include "stringFunctions.h"
#include "macros.h"
#include "imageHelper.h"
#include <string.h>
#include <stdlib.h>

/// @brief checks if the file exists
/// @param filePath the path to the file
/// @return true if yes and false if no
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
/// @brief gets the size of a file
/// @param file the file
/// @return the size of the file
size_t fileSize(FILE* file)
{
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}
/// @brief gets file name from its path
/// @param filePath the file's path
/// @return the file's name
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
/// @brief gets a file's format from its path
/// @param filePath the file path
/// @return the file's format
char* getFileFormat(char* filePath)
{
    char* format = NULL;
    int dotPosition = findLast(filePath,'.') + 1;
    if (dotPosition == 0)
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
/// @brief creates the file object
/// @param filePath the path to the file
/// @return pointer to the file object
FileObject* createFileObject(char* filePath)
{
    FILE* file = NULL;
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
    file = fopen(filePath,READ_BINARY);
    if (file == NULL)
    {
        LOG_ERROR("Could not open the file: %s",filePath);
        free(fileObject);
        return NULL;
    }
    fileObject->m_name = getFileName(filePath);
    if (fileObject->m_name == NULL)
    {
        LOG_ERROR("Could not get the file's name");
        fclose(file);
        free(fileObject);
        return NULL;
    }
    fileObject->m_format = getFileFormat(filePath);
    if (fileObject->m_format == NULL)
    {
        LOG_ERROR("Could not get the file's format");
        fclose(file);
        free(fileObject->m_name);
        free(fileObject);
        return NULL;
    }
    fileObject->m_size = fileSize(file);
    //Uncompressing the image if needed(for formats like png)
    fileObject->m_data = loadImage(filePath, &fileObject->m_width, &fileObject->m_height, &fileObject->m_channels);
    if (fileObject->m_data == NULL)
    {
        LOG_ERROR("Could not load image");
        fclose(file);
        free(fileObject->m_name);
        free(fileObject->m_format);
        free(fileObject);
        return NULL;
    }
    fileObject->m_path = (char*)malloc(strlen(filePath) + 1);
    if (fileObject->m_path == NULL)
    {
        LOG_ERROR("Could not allocate memory for the file's path");
        fclose(file);
        free(fileObject->m_name);
        free(fileObject->m_format);
        free(fileObject->m_data);
        free(fileObject);
        return NULL;
    }
    strcpy(fileObject->m_path,filePath);
    fclose(file);
    return fileObject;
}
/// @brief free all allocated memory for a file object
/// @param fileObject pointer to a file object
void freeFileObject(FileObject* fileObject)
{
    freeImageData(fileObject->m_data);
    free(fileObject->m_format);
    free(fileObject->m_name);
    free(fileObject->m_path);
    free(fileObject);
}
