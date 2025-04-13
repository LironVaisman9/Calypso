#include "UI.h"
#include "macros.h"
#include "fileManager.h"
#include "steganography.h"
#include <string.h>

void printManuel()
{
    printf("Welcome to Calypso!\n");
    printf("Usage: Calypso <action> <path>\n");
    printf("-e to encode into a file\n");
    printf("-d to decode from a file\n");
}
void handleExe(int argc,char** argv)
{
    if (argc != 3 || ((strcmp(argv[1],"-e") != 0) && strcmp(argv[1],"-d") != 0))
    {
        if (argc != 2 || strcmp(argv[1],"-help") != 0)
        {
            LOG_ERROR("Invalid input.\nUse -help to print manual\n");
            return;
        }
        printManuel();
        return;
    }
    if (!fileExist(argv[2]))
    {
        LOG_ERROR("File does not exist");
        return;
    }
    FileObject* file = createFileObject(argv[2]);
    if (file == NULL)
    {
        LOG_ERROR("Could not create file object");
        return;
    }
    if (strcmp(argv[1],"-e") == 0)
    {
       
       encode(file);
       return;
    }
    decode(file);
}