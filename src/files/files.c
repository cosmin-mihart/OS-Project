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
            printf("-a: access rights\n");
            printf("-d: size\n");
            printf("-h: hard link count\n");
            printf("-l: create symbolic link\n");
            printf("-m: last modified\n");
            printf("-n: name\n");
            printf("\nPlease give some options (Your input should begin with \'-\'): ");
            scanf("%s", options);
            break;
        case SYMLINK:
            printf("%s - SYMBOLIC LINk\n", fileName);
            printf("--- OPTIONS ---\n");
            printf("-a: access rights\n");
            printf("-d: size of symbolic link\n");
            printf("-l: delete symbolic link\n");
            printf("-n: name\n");
            printf("-t: size of the target file\n");
            printf("\nPlease give some options (Your input should begin with \'-\'): ");
            scanf("%s", options);
            break;
        case DIRECTORY:
            printf("%s - DIRECTORY\n", fileName);
            printf("--- OPTIONS ---\n");
            printf("-a: access rights\n");
            printf("-d: size of symbolic link\n");
            printf("-l: delete symbolic link\n");
            printf("-n: name\n");
            printf("-t: size of the target file\n");
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
    struct stat st;
    switch (fileType) {
        case REGULAR:
            stat(fileName, &st);
            break;
        case SYMLINK:
            lstat(fileName, &st);
            break;
    }
    for (int i = 0; operations[i] != '\0'; i++) {
        switch (fileType) {
            case REGULAR:
                switch (operations[i]) {
                    case 'a':
                        printAccessRights(st);
                        break;
                    case 'd':
                        printSize(st);
                        break;
                    case 'h':
                        printHardLinkCount(st);
                        break;
                    case 'l':
                        createSymbolicLink();
                        break;
                    case 'm':
                        printTimeOfLastModification(st);
                        break;
                    case 'n':
                        printf("Name of file: %s\n", fileName);
                        break;
                    default:
                }
                break;
            case SYMLINK:
                break;
        }
    }
}

void parseArguments(int argc, const char *argv[]) {
    for (int i = 1; i < argc; i++) {
        struct stat st;
        stat(argv[i], &st);
        if (S_ISREG(st.st_mode)) {
            char *operations = printMenu(REGULAR, argv[i]);
            runOperations(REGULAR, argv[i], operations);
        } else if (S_ISLNK(st.st_mode)) {
            char *operations = printMenu(SYMLINK, argv[i]);
            runOperations(SYMLINK, argv[i], operations);
        } else if (S_ISDIR(st.st_mode)) {
            char *operations = printMenu(DIRECTORY, argv[i]);
            runOperations(DIRECTORY, argv[i], operations);
        }
    }
}

void printHardLinkCount(struct stat s) {
    printf("Hard Link Count: %lu\n", s.st_nlink);
}

void printAccessRights(struct stat s) {
    mode_t mode = s.st_mode;
    printf("Access rights:\n");
    printf("  User:\n");
    printf("\tRead - %s\n", mode & S_IRUSR ? "yes" : "no");
    printf("\tWrite - %s\n", mode & S_IWUSR ? "yes" : "no");
    printf("\tExecute - %s\n", mode & S_IXUSR ? "yes" : "no");
    printf("  Group:\n");
    printf("\tRead - %s\n", mode & S_IRGRP ? "yes" : "no");
    printf("\tWrite - %s\n", mode & S_IWGRP ? "yes" : "no");
    printf("\tExecute - %s\n", mode & S_IXGRP ? "yes" : "no");
    printf("  Others:\n");
    printf("\tRead - %s\n", mode & S_IROTH ? "yes" : "no");
    printf("\tWrite - %s\n", mode & S_IWOTH ? "yes" : "no");
    printf("\tExecute - %s\n", mode & S_IXOTH ? "yes" : "no");
}

void printSize(struct stat s) {
    printf("Size on disk: %lu kb\n", s.st_size);
}

void createSymbolicLink() {

}

void printTimeOfLastModification(struct stat s) {

}