#include "steganography.h"
#include "macros.h"
#include "stringFunctions.h"
#include "imageHelper.h"
#include <stdlib.h>
#include <string.h>

/// @brief finds a random starting position for LSB encoding
/// @param data the binary data of a file
/// @param dataSize the size of the binary data
/// @param length the length of the txt that will be encoded
/// @return the starting position
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
/// @brief creates a magic signature
/// @param file the file that the magic will be based on
/// @return the magic signature
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
/// @brief checks if a header is valid
/// @param header the header
/// @param magic the magic signature of the file
/// @return true if yes and false if no
bool headerValid(Header* header,char* magic)
{
    return strcmp(header->m_magic,magic) == 0;
}
/// @brief creates an header for LSB encoding
/// @param file the file that the header will be based on
/// @param textSize the size of the text that will be encoded
/// @param startingPos the starting position of the LSB encoding
/// @return the header
Header* createHeader(FileObject* file,uint16_t textSize,size_t startingPos)
{
    Header* header = (Header*)malloc(sizeof(Header));
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
    header->m_textSize = textSize;
    header->m_startPos = startingPos;
    return header;
}
/// @brief insert header into binary data
/// @param dest the binary data of the dest file
/// @param header the header
/// @return true if process was successful and false if no
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
/// @brief gets a header from binary data if it exists
/// @param src the binary data
/// @return the header(NULL if it was not found)
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
/// @brief checks if a tail is valid
/// @param tail the tail
/// @param magic the magic signature of the file
/// @return true if the tail is valid and false if no
bool tailValid(Tail* tail,char* magic)
{
    return strcmp(tail->m_magic,magic) == 0;
}
/// @brief gets a tail from binary data if it exists
/// @param src the binary data
/// @param size the size of the binary data
/// @return the tail(NULL if it was not found)
Tail* getTail(unsigned char* src,size_t size)
{
    Tail* tail = NULL;
    unsigned char* tailPos = src + size - sizeof(Tail);
    tail = (Tail*)malloc(sizeof(Tail));
    if (tail == NULL)
    {
        LOG_ERROR("Could not allocate memory for tail");
        return NULL;
    }
    memcpy(tail, tailPos, sizeof(Tail));
    return tail;
}
/// @brief creates a tail for file encoding
/// @param file the file that the tail will be based on
/// @param format the format of the file that will be encoded
/// @param offset the offset to the start of the file
/// @param size the size of the file that will be encoded
/// @return the tail
Tail* createTail(FileObject* file,char* format,size_t offset,size_t size)
{
    Tail* tail = (Tail*)malloc(sizeof(Tail));
    if (tail == NULL)
    {
        LOG_ERROR("Could not allocate memory for tail");
        return NULL;
    }
    char* magic = createMagic(file);
    if (magic == NULL)
    {
        LOG_ERROR("Could not create magic for tail");
        free(tail);
        return NULL;
    }
    strcpy(tail->m_magic,magic);
    free(magic);
    strcpy(tail->m_format,format);
    tail->m_offset = offset;
    tail->m_size = size;
    return tail;
}
/// @brief encodes binary data using LSB encoding
/// @param dest the dest binary data
/// @param data the binary data that will be encoded
/// @param startPos the starting position of the encoding
/// @param size the size of the binary data that will be encoded
/// @return true if the process was successful and false if no
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
            //Writes the bit into the LSB of the spesific byte
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
/// @brief decodes binary data that was encoded using LSB encoding
/// @param src the binary data where the binary is hidden
/// @param dest where the binary data will be saved
/// @param startPos the starting position of the encoding
/// @param size the size of the binary data that was encoded
/// @return true if the process was successful and false if no
bool decodeLSB(unsigned char* src,unsigned char* dest,int startPos,int size)
{
    int bitsRead = 0;
    int i = 0;
    for (i = 0; i < size; i++)
    {
        unsigned char byte = 0;
        int bit = 0;
        //Reads one LSB from every byte into the dest
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
/// @brief encodes a message into a file using LSB encoding
/// @param destFile the file that will hide the message
/// @param msg the message that will be encoded
/// @return true if the process was successful and false if no
bool encodeMessage(FileObject* destFile,char* msg)
{
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
    Header* header = createHeader(destFile,strlen(msg),startingPos);
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
    if(!writeToImage(destFile->m_format,destFile->m_path,destFile->m_data,destFile->m_width,destFile->m_height,destFile->m_channels))
    {
        LOG_ERROR("Could not write to image");
        free(binaryMsg);
        return false;
    }
    free(binaryMsg);
    return true;
}
/// @brief decodes a message that was hidden using LSB encoding
/// @param srcFile the file where the msg is hidden
/// @param msg pointer to char* where the message will be saved
/// @return true if the process was successful and false if no
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
/// @brief encodes a file into the end of a image
/// @param destFile the image that will hide the file
/// @param file the file that will be encoded
/// @return true if the process was successful and false if no
bool encodeFile(FileObject* destFile,FileObject* file)
{
    Tail* tail = NULL;
    unsigned char* dataToEncode = NULL;
    FILE* out = fopen(destFile->m_path,APPEND_BINARY);
    if (out == NULL)
    {
        LOG_ERROR("Could not open file in: %s",destFile->m_path);
        return false;
    }
    dataToEncode = (unsigned char*)malloc(file->m_size);
    if (dataToEncode == NULL)
    {
        LOG_ERROR("Could not allocate memory for the data that will be encoded");
        fclose(out);
        return false;
    }
    FILE* in = fopen(file->m_path,READ_BINARY);
    if (in == NULL)
    {
        LOG_ERROR("Could not opem file in: %s",file->m_path);
        free(dataToEncode);
        fclose(out);
        return false;
    }
    fread(dataToEncode,file->m_size,1,in);
    fclose(in);
    fwrite(dataToEncode, 1, file->m_size, out);
    free(dataToEncode);
    tail = createTail(destFile,file->m_format,destFile->m_size,file->m_size);
    if (tail == NULL)
    {
        LOG_ERROR("Could not allocate memory for tail");
        fclose(out);
        return false;
    }
    fwrite(tail, sizeof(Tail), 1, out);
    fclose(out);
    free(tail);
    return true;
}
/// @brief decodes a file that was hidden in the end of a image
/// @param srcFile the image that hides the file
/// @param path path to where the file will be saved
/// @return true if the process was successful and false if no
bool decodeFile(FileObject* srcFile,char* path)
{
    Tail* tail = NULL;
    unsigned char* decodedData = NULL;
    unsigned char* fileData = NULL;
    FILE* file = fopen(srcFile->m_path,READ_BINARY);
    if (file == NULL)
    {
        LOG_ERROR("Could not open file in: %s",srcFile->m_path);
        return false;
    }
    fileData = (unsigned char*)malloc(srcFile->m_size);
    if (fileData == NULL)
    {
        LOG_ERROR("Could not allocate memory for file data");
        fclose(file);
        return false;
    }
    fread(fileData,srcFile->m_size,1,file);
    fclose(file);
    tail = getTail(fileData,srcFile->m_size);
    if (tail == NULL)
    {
        LOG_ERROR("Could not get tail. file may not be encoded");
        free(fileData);
        return false;
    }
    char* magic = createMagic(srcFile);
    if (magic == NULL)
    {
        LOG_ERROR("Could not create magic for testing");
        free(fileData);
        return false;
    }
    if (!tailValid(tail,magic))
    {
        LOG_ERROR("Tail is not valid. file may not be encoded");
        free(fileData);
        free(magic);
        return false;
    }
    free(magic);
    decodedData = (unsigned char*)malloc(tail->m_size);
    if (decodedData == NULL)
    {
        LOG_ERROR("Could not allocate memory for decoded file data");
        free(fileData);
        return false;
    }
    memcpy(decodedData, fileData + tail->m_offset, tail->m_size);
    char* fullPath = combinePath(path,tail->m_format);
    if (fullPath == NULL)
    {
        LOG_ERROR("Could not create full path for decoded file");
        free(fileData);
        return false;
    }
    free(fileData);
    FILE* out = fopen(fullPath,WRITE_BINARY);
    if (out == NULL)
    {
        LOG_ERROR("Could not create file in: %s",fullPath);
        free(fullPath);
        return false;
    }
    fwrite(decodedData,tail->m_size,1,out);
    free(fullPath);
    fclose(out);
    free(decodedData);
    return true;
}
/// @brief cehcks if a file has LSB encoding header
/// @param file the file
/// @return true if it has and false if no
bool hasHeader(FileObject* file)
{
    Header* header = getHeader(file->m_data);
    if (header == NULL)
    {
        return false;
    }
    char* magic = createMagic(file);
    if (magic == NULL)
    {
        LOG_ERROR("Could not create magic for testing");
        free(header);
        return false;
    }
    if (!headerValid(header,magic))
    {
        free(header);
        free(magic);
        return false;
    }
    free(header);
    free(magic);
    return true;
}
/// @brief checks if a file has tail of file encoding
/// @param file the file
/// @return true if yes and false if no
bool hasTail(FileObject* file)
{
    FILE* in = fopen(file->m_path,READ_BINARY);
    if (in == NULL)
    {
        LOG_ERROR("Could not open file in: %s",file->m_path);
        return false;
    }
    unsigned char* data = (unsigned char*)malloc(file->m_size);
    if (data == NULL)
    {
        LOG_ERROR("Could not allocate memory for testing data");
        fclose(in);
        return false;
    }
    fread(data,file->m_size,1,in);
    Tail* tail = getTail(data,file->m_size);
    free(data);
    fclose(in);
    if (tail == NULL)
    {
        return false;
    }
    char* magic = createMagic(file);
    if (magic == NULL)
    {
        LOG_ERROR("Could not create magic for testing");
        free(tail);
        return false;
    }
    if (!tailValid(tail,magic))
    {
        free(tail);
        free(magic);
        return false;
    }
    free(tail);
    free(magic);
    return true;
}
/// @brief encodes data into a file based on user input
/// @param file the file
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
        if(!getUserInput(text,MAX_TEXT_LENGTH,"Enter the text that you want to encode: "))
        {
            LOG_ERROR("Could not get user input");
            free(text);
            return;
        }
        if(!encodeMessage(file,text))
        {
            LOG_ERROR("Could not encode message");
        }
        free(text);
        break;
    case ENCODE_FILE:
        char* path = (char*)malloc(sizeof(char) * (MAX_PATH_LENGTH));
        if (path == NULL)
        {
            LOG_ERROR("Could not allocate memory for user input");
            return;
        }
        if(!getUserInput(path,MAX_PATH_LENGTH,"Enter the path to the file you want to encode: "))
        {
            LOG_ERROR("Could not get user input");
            free(path);
            return;
        }
        FileObject* fileObject = createFileObject(path);
        if (fileObject == NULL)
        {
            LOG_ERROR("Could not create FileObject for the file in: %s",path);
            free(path);
            return;
        }
        if(!encodeFile(file,fileObject))
        {
            LOG_ERROR("Could not encode file");
        }
        free(path);
        freeFileObject(fileObject);
        break;
    default:
        LOG_ERROR("Unsupported option");
        break;
    }
}
/// @brief decodes data from a file based on user input
/// @param file the file
void decode(FileObject* file)
{
    if (hasHeader(file))
    {
        char* msg = NULL;
        if(!decodeMessage(file,&msg))
        {
            LOG_ERROR("Failed to decode message");
            return;
        }
        printf("%s\n",msg);
        free(msg);
        return;
    }
    else if(hasTail(file))
    {
        char* path = NULL;
        path = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
        if (path == NULL)
        {
            LOG_ERROR("Could not allocate memory for path for the decoded file");
            return;
        }
        if(!getUserInput(path,MAX_PATH_LENGTH,"Enter path where the file will be saved(include a name for the file): "))
        {
            LOG_ERROR("Could not get user input");
            free(path);
            return;
        }
        if(!decodeFile(file,path))
        {
            LOG_ERROR("Failed to decode file");
        }
        free(path);
        return;
    }
    LOG_ERROR("File is not encoded");
}