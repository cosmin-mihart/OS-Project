#include <sys/stat.h>

#ifndef FILES_H
#define FILES_H

enum FileType { REGULAR = 0, SYMLINK, DIRECTORY };

char *printMenu(enum FileType fileType, const char *fileName);
int validOperations(const char *fileTypeOptions, const char *option);
void createSymbolicLink(const char *fileName);
void parseArguments(int argc, const char *argv[]);
void printAccessRights(struct stat st);
void printHardLinkCount(struct stat st);
off_t getDirectorySize(const char *dir_path);
char *humanReadableSize(long size);
void printSize(struct stat st);
void printTimeOfLastModification(struct stat st);
void runOperations(enum FileType fileType, const char *fileName,
                   const char *operations);

#endif // FILES_H
