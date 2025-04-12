#include "steganography.h"
#include "macros.h"
#include "stringFunctions.h"
#include "imageHelper.h"
#include <stdlib.h>
#include <string.h>

size_t findstartingPos(unsigned char* data,size_t dataSize,uint16_t length)
{
    size_t startingPos = 0;
    size_t bytesToWrite = length * 8;
    size_t endOfHeader = HEADER_STARTING_BYTE + sizeof(Header);
    if (dataSize < (endOfHeader + bytesToWrite))
    {
        LOG_ERROR("The binary data is to small");
        return 0;
    }
    size_t maxStartingPos = dataSize - bytesToWrite;
    if (maxStartingPos <= endOfHeader)
    {
        LOG_ERROR("Not enough space after header in data");
        return 0;
    }
    size_t minStartingPos = endOfHeader + 1;
    size_t range = maxStartingPos - minStartingPos;
    startingPos = minStartingPos + rand() % range;
    return startingPos;
}

char* createMagic(FileObject* file)
{
    char* magic = NULL;
    magic = (char*)malloc(sizeof(char) * (MAGIC_LENGTH + 1));
    if (magic == NULL)
    {
        LOG_ERROR("Could not allocate memory for magic");
        return NULL;
    }
    unsigned int seed = (unsigned int)strlen(file->m_name);
    randomString(magic,MAGIC_LENGTH + 1,seed,true);
    return magic;
}

bool headerValid(Header* header,char* magic)
{
    if (strcmp(header->m_magic,magic) != 0)
    {
        return false;
    }
    if (!(header->m_type == 't' || header->m_type == 'f'))
    {
        return false;
    }
    return true;
}

Header* createHeader(FileObject* file,char type,char* fileFormat,uint16_t textSize,size_t startingPos)
{
    Header* header = (Header*)malloc(sizeof(header));
    if (header == NULL)
    {
        LOG_ERROR("Could not allocate memory for the header");
        return NULL;
    }
    char* magic = createMagic(file);
    if (magic == NULL)
    {
        LOG_ERROR("Could not create magic");
        return NULL;
    }
    strcpy(header->m_magic,magic);
    free(magic);
    header->m_type = type;
    strcpy(header->m_fileFormat,fileFormat);
    header->m_textSize = textSize;
    header->m_startPos = startingPos;
    return header;
}

bool insertHeader(unsigned char* dest,Header* header)
{
    unsigned char* binaryHeader = (unsigned char*)malloc(sizeof(Header));
    if (binaryHeader == NULL)
    {
        LOG_ERROR("Could not allocate memory for binary header");
        return false;
    }
    memcpy(binaryHeader, header, sizeof(Header));
    if(!encodeLSB(dest,binaryHeader,HEADER_STARTING_BYTE,sizeof(Header)))
    {
        LOG_ERROR("Could not encode header into the image");
        free(binaryHeader);
        return false;
    }
    free(binaryHeader);
    return true;
}
Header* getHeader(unsigned char* src)
{
    Header* header = NULL;
    unsigned char* binaryHeader = NULL;
    binaryHeader = (unsigned char*)malloc(sizeof(Header));
    if (binaryHeader == NULL)
    {
        LOG_ERROR("Could not allocate memory for binary header");
        return NULL;
    }
    if (!decodeLSB(src,binaryHeader,HEADER_STARTING_BYTE,sizeof(Header)))
    {
        LOG_ERROR("Could not read steganography from file");
        free(binaryHeader);
        return false;
    }
    header = (Header*)malloc(sizeof(Header));
    if (header == NULL)
    {
        LOG_ERROR("Could not allocate memory for the steganography header");
        free(binaryHeader);
        return NULL;
    }
    memcpy(header, binaryHeader, sizeof(Header));
    free(binaryHeader);
    return header;
}
bool encodeLSB(unsigned char* dest,unsigned char* data,int startPos,int size)
{
    int bitsWritten = 0;
    int i = 0;
    for (i = 0; i < size; i++)
    {
        unsigned char byte = data[i];
        int bit = 0;
        for (bit = 7; bit >= 0; bit--)
        {
            int bitToWrite = (byte >> bit) & 1;
            dest[startPos + bitsWritten] &= 0xFE;
            dest[startPos + bitsWritten] |= bitToWrite;
            bitsWritten++;
        }
    }
    if (bitsWritten != (size * 8))
    {
        LOG_ERROR("Could not write the entire data to the dest");
        return false;
    }
    return true;
}
bool decodeLSB(unsigned char* src,unsigned char* dest,int startPos,int size)
{
    int bitsRead = 0;
    int i = 0;
    for (i = 0; i < size; i++)
    {
        unsigned char byte = 0;
        int bit = 0;
        for (bit = 7; bit >= 0; bit--)
        {
            int bitFromSrc = src[startPos + bitsRead] & 1;
            byte |= (bitFromSrc << bit);
            bitsRead++;
        }
        dest[i] = byte;
    }
    if (bitsRead != (size * 8))
    {
        LOG_ERROR("Could not read all the binary data from src");
        return false;
    }
    return true;
}

bool encodeMessage(FileObject* destFile,char* msg)
{
    FILE* fileToWrite = NULL;
    unsigned char* binaryMsg = NULL;
    size_t startingPos = findstartingPos(destFile->m_data,destFile->m_size,strlen(msg));
    if (startingPos == 0)
    {
        LOG_ERROR("Could not find starting byte for the encoding");
        return false;
    }
    char* junkData = (char*)malloc(sizeof(char) * MAX_FORMAT_LENGTH);
    if (junkData == NULL)
    {
        LOG_ERROR("Could not allocate memory for junk data");
        return false;
    }
    unsigned int seed = (unsigned int)strlen(destFile->m_format);
    randomString(junkData,MAX_FORMAT_LENGTH,seed,true);
    Header* header = createHeader(destFile,'t',junkData,strlen(msg),startingPos);
    free(junkData);
    if (header == NULL)
    {
        LOG_ERROR("Could not create header for the encoding");
        return false;
    }
    if(!insertHeader(destFile->m_data,header))
    {
        LOG_ERROR("Could not insert Header");
        free(header);
        return false;
    }
    free(header);
    binaryMsg = stringToBinary(msg,strlen(msg) + 1);
    if (binaryMsg == NULL)
    {
        LOG_ERROR("Could not convert the msg to binary");
        return false;
    }
    if (!encodeLSB(destFile->m_data,binaryMsg,startingPos,strlen(msg) + 1))
    {
        LOG_ERROR("Could not encode the binary data into the file");
        free(binaryMsg);
        return false;
    }
    fileToWrite = fopen(destFile->m_path,WRITE_BINARY);
    if (fileToWrite == NULL)
    {
        LOG_ERROR("Could not open file for writing");
        free(binaryMsg);
        return false;
    }
    writeToImage(destFile->m_format,destFile->m_path,destFile->m_data,destFile->m_width,destFile->m_height,destFile->m_channels);
    fclose(fileToWrite);
    free(binaryMsg);
    return true;
}
bool decodeMessage(FileObject* srcFile,char** msg)
{
    unsigned char* binaryMsg = NULL;
    Header* header = getHeader(srcFile->m_data);
    if (header == NULL)
    {
        LOG_ERROR("Could not get header from src file. The file may be not encoded");
        return false;
    }
    char* magic = createMagic(srcFile);
    if (magic == false)
    {
        LOG_ERROR("Could not create magic for testing header");
        free(header);
        return false;
    }
    if (!headerValid(header,magic))
    {
        LOG_ERROR("Header is not valid. The file may be not encoded");
        free(header);
        return false;
    }
    free(magic);
    binaryMsg = (unsigned char*)malloc(header->m_textSize);
    if (binaryMsg == NULL)
    {
        LOG_ERROR("Could not allocate memory for binary msg");
        free(header);
        return false;
    }
    if (!decodeLSB(srcFile->m_data,binaryMsg,header->m_startPos,header->m_textSize))
    {
        LOG_ERROR("Could not decode binary data from the src file");
        free(header);
        free(binaryMsg);
        return false;
    }
    *msg = binaryToString(binaryMsg,header->m_textSize);
    if (*msg == NULL)
    {
        LOG_ERROR("Could not convert the binary data to a string");
        free(header);
        free(binaryMsg);
        return false;
    }
    free(header);
    free(binaryMsg);
    return true;
}
bool encodeFile(FileObject* destFile,FileObject* file)
{
    FILE* fileToWrite = NULL;
    uint16_t junkData = (uint16_t)strlen(file->m_path);
    Header* header = createHeader(destFile,'f',file->m_format,junkData,destFile->m_size);
    if (header == NULL)
    {
        LOG_ERROR("Could not create header for the encoding");
        return false;
    }
    if (!insertHeader(destFile->m_data,header))
    {
        LOG_ERROR("Could not insert Header");
        free(header);
        return false;
    }
    printf("%zu\n",destFile->m_size);
    printf("%zu\n",file->m_size);
    size_t newFileSize = destFile->m_size + file->m_size;
    printf("%zu\n",newFileSize);
    unsigned char* newFileData = (unsigned char*)malloc(newFileSize);
    if (newFileData == NULL)
    {
        LOG_ERROR("Could not allocate memory for the new file data");
        free(header);
        return false;
    }
    memcpy(newFileData, destFile->m_data, destFile->m_size);
    memcpy(newFileData + destFile->m_size, file->m_data, file->m_size);
    fileToWrite = fopen(destFile->m_path,WRITE_BINARY);
    if (fileToWrite == NULL)
    {
        LOG_ERROR("Could not open file for writing");
        free(header);
        return false;
    }
    writeToImage(destFile->m_format,destFile->m_path,newFileData,destFile->m_width,destFile->m_height,destFile->m_channels);
    free(destFile->m_data);
    destFile->m_data = newFileData;
    destFile->m_size = newFileSize;
    free(header);
    fclose(fileToWrite);
    return true;
}
bool decodeFile(FileObject* srcFile,char* path)
{
    unsigned char* fileData = NULL;
    char* fullPath = NULL;
    Header* header = getHeader(srcFile->m_data);
    if (header == NULL)
    {
        LOG_ERROR("Could not get header from src file. The file may be not encoded");
        return false;
    }
    char* magic = createMagic(srcFile);
    if (magic == false)
    {
        LOG_ERROR("Could not create magic for testing header");
        free(header);
        return false;
    }
    if (!headerValid(header,magic))
    {
        LOG_ERROR("Header is not valid. The file may be not encoded");
        free(header);
        return false;
    }
    free(magic);
    size_t fileSize = srcFile->m_size - header->m_startPos;
    printf("%zu",fileSize);
    fileData = (unsigned char*)malloc(fileSize);
    if (fileData == NULL)
    {
        LOG_ERROR("Could not allocate memory for file data");
        free(header);
        return false;
    }
    fullPath = combinePath(path,header->m_fileFormat);
    if (fullPath == NULL)
    {
        LOG_ERROR("Could not create full path for the decoded file");
        free(header);
        return false;
    }
    memcpy(fileData, srcFile->m_data + header->m_startPos, fileSize);
    FILE* outFile = fopen(fullPath, WRITE_BINARY);
    if (outFile == NULL)
    {
        LOG_ERROR("Could not create file in: %s",fullPath);
        free(header);
        free(fileData);
        return false;
    }
    fwrite(fileData, 1, fileSize, outFile);
    fclose(outFile);
    free(header);
    free(fileData);
    return true;
}
void encode(FileObject* file)
{
    int option = 0;
    do
    {
        printf("Encoding options:\n");
        printf("[1] Encode text\n");
        printf("[2] Encode file\n");
        scanf("%d",&option);
        getchar();
    } while (option != ENCODE_TEXT && option != ENCODE_FILE);
    switch (option)
    {
    case ENCODE_TEXT:
        char* text = (char*)malloc(sizeof(char) * (MAX_TEXT_LENGTH));
        if (text == NULL)
        {
            LOG_ERROR("Could not allocate memory for user input");
            return;
        }
        getUserInput(text,MAX_TEXT_LENGTH,"Enter the text that you want to encode: ");
        encodeMessage(file,text);
        free(text);
        break;
    case ENCODE_FILE:
        char* path = (char*)malloc(sizeof(char) * (MAX_PATH_LENGTH));
        if (path == NULL)
        {
            LOG_ERROR("Could not allocate memory for user input");
            return;
        }
        getUserInput(path,MAX_PATH_LENGTH,"Enter the path to the file you want to encode: ");
        FileObject* fileObject = createFileObject(path);
        if (fileObject == NULL)
        {
            LOG_ERROR("Could not create FileObject for the file in: %s",path);
            free(path);
            return;
        }
        encodeFile(file,fileObject);
        free(path);
        freeFileObject(fileObject);
        break;
    default:
        LOG_ERROR("Unsupported option");
        break;
    }
}