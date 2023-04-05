#include <sys/stat.h>

#ifndef FILES_H
#define FILES_H

enum FileType {
    REGULAR = 0,
    SYMLINK
};

char *printMenu(enum FileType fileType, const char* fileName);
int  validOperations(enum FileType, const char *option);
void parseArguments(int argc, const char *argv[]);
void runOperations(enum FileType fileType, const char *fileName, const char *operations);
void printName(struct stat s);
void printHardLinkCount(struct stat s);
void printAccessRights(struct stat s);
void printSize(struct stat s);
void createSymbolicLink();

#endif //FILES_H