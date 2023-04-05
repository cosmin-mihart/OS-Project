#include <stdio.h>
#include <sys/stat.h>
#include <files/files.h>
#include <stdlib.h>
#include <string.h>

char *printMenu(enum FileType fileType, const char *fileName) {
    char *options = malloc(32);
    switch (fileType) {
        case REGULAR:
            printf("%s - REGULAR FILE\n", fileName);
            printf("--- OPTIONS ---\n");
            printf("\t-a: access rights\n");
            printf("\t-d: size\n");
            printf("\t-h: hard link count\n");
            printf("\t-l: create symbolic link\n");
            printf("\t-m: last modified\n");
            printf("\t-n: name\n");
            printf("\nPlease give some options (Your input should begin with \'-\'): ");
            scanf("%s", options);
            break;
        case SYMLINK:
            printf("%s - SYMBOLIC LINk\n", fileName);
            printf("--- OPTIONS ---\n");
            printf("\t-a: access rights\n");
            printf("\t-d: size of symbolic link\n");
            printf("\t-l: delete symbolic link\n");
            printf("\t-n: name\n");
            printf("\t-t: size of the target file\n");
            printf("\nPlease give some options (Your input should begin with \'-\'): ");
            scanf("%s", options);
            break;
    }
    if (!validOperations(fileType, options)) {
        return printMenu(fileType, fileName);
    }

    return options;
}

int  validOperations(enum FileType fileType, const char *options) {
    switch (fileType) {
        case REGULAR: {
            const char regularFileOptions[] = "-adhlmn";
            for (int i = 0; regularFileOptions[i] != '\0'; i++) {
                if (!strchr(regularFileOptions, options[i])) {
                    printf("\'%c\' is an invalid option!\n", options[i]);
                    return 0;
                }
            }

            break;
        }
        case SYMLINK: {
            const char symlinkOptions[] = "-adlnt";
            for (int i = 0; symlinkOptions[i] != '\0'; i++) {
                if (!strchr(symlinkOptions, options[i])) {
                    printf("\'%c\' is an invalid option!\n", options[i]);
                    return 0;
                }
            }

            break;
        }
    }

    return 1;
}

void runOperations(enum FileType fileType, const char *fileName, const char *operations) {
    for (int i = 0; operations[i] != '\0'; i++) {

    }
}

void parseArguments(int argc, const char *argv[]) {

}

void printName(struct stat s) {

}

void printHardLinkCount(struct stat s) {

}

void printAccessRights(struct stat s) {

}

void printSize(struct stat s) {

}

void createSymbolicLink() {

}