#include <dirent.h>
#include <files/files.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

char *printMenu(enum FileType fileType, const char *fileName) {
    char *options = malloc(32);
    switch (fileType) {
        case REGULAR:
            printf("\n%s - REGULAR FILE\n", fileName);
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
            printf("\n%s - SYMBOLIC LINk\n", fileName);
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
            printf("\n%s - DIRECTORY\n", fileName);
            printf("--- OPTIONS ---\n");
            printf("-a: access rights\n");
            printf("-c: number of .c files\n");
            printf("-d: size of directory\n");
            printf("-n: name\n");
            printf("\nPlease give some options (Your input should begin with \'-\'): ");
            scanf("%s", options);
            break;
    }

    int valid = 0;
    switch (fileType) {
        case REGULAR:
            valid = validOperations("-adhlmn", options);
            break;
        case SYMLINK:
            valid = validOperations("-adlnt", options);
            break;
        case DIRECTORY:
            valid = validOperations("-acdn", options);
            break;
    }

    if (!valid) {
        free(options);
        return printMenu(fileType, fileName);
    }

    return options;
}

int validOperations(const char *fileTypeOptions, const char *options) {
    for (int i = 0; options[i] != '\0'; i++) {
        if (!strchr(fileTypeOptions, options[i])) {
            printf("\'%c\' is an invalid option!\n", options[i]);
            return 0;
        }
    }

    return 1;
}

void runOperations(enum FileType fileType, const char *fileName,
                   const char *operations) {
    struct stat st;
    switch (fileType) {
        case REGULAR:
            stat(fileName, &st);
            break;
        case SYMLINK:
            lstat(fileName, &st);
            break;
        case DIRECTORY:
            break;
    }
    for (int i = 0; operations[i] != '\0'; i++) {
        switch (fileType) {
            case SYMLINK:
                switch (operations[i]) {
                    case 'a':
                        printAccessRights(st);
                        break;
                    case 'd':
                        printSize(st);
                        break;
                    case 'l': {
                        unlink(fileName);
                        printf("Deleted symlink \'%s\'.\n", fileName);
                        i = strlen(operations);
                        break;
                    }
                    case 'n':
                        printf("Name of file: %s\n", fileName);
                        break;
                    case 't': {
                        struct stat s;
                        stat(fileName, &s);
                        printSize(s);
                        break;
                    }
                }
                break;
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
                        createSymbolicLink(fileName);
                        break;
                    case 'm':
                        printTimeOfLastModification(st);
                        break;
                    case 'n':
                        printf("Name of file: %s\n", fileName);
                        break;
                }
                break;
            case DIRECTORY:
                switch (operations[i]) {
                    case 'a':
                        printAccessRights(st);
                        break;
                    case 'c': {
                        DIR *dir;
                        struct dirent *ent;
                        const char *path = fileName;
                        int count = 0;

                        dir = opendir(path);

                        while ((ent = readdir(dir)) != NULL) {
                            if (ent->d_type == DT_REG && strstr(ent->d_name, ".c") != NULL) {
                                count++;
                            }
                        }
                        closedir(dir);

                        printf("Number of .c files: %d\n", count);
                        break;
                    }
                    case 'd':
                        printf("Size on disk: %s\n",
                               humanReadableSize(getDirectorySize(fileName)));
                        break;
                    case 'n':
                        printf("Name of file: %s\n", fileName);
                        break;
                }
                break;
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

off_t getDirectorySize(const char *dir_path) {
    struct stat st;
    off_t total_size = 0;

    if (stat(dir_path, &st) != 0) {
        fprintf(stderr, "Unable to get information about %s\n", dir_path);
        return -1;
    }

    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "%s is not a directory\n", dir_path);
        return -1;
    }

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        fprintf(stderr, "Unable to open directory %s\n", dir_path);
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            // Regular file
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);
            struct stat file_st;
            if (stat(file_path, &file_st) == 0) {
                total_size += file_st.st_size;
            } else {
                fprintf(stderr, "Unable to get size of file %s\n", file_path);
            }
        } else if (entry->d_type == DT_DIR) {
            // Directory
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char subdir_path[1024];
                snprintf(subdir_path, sizeof(subdir_path), "%s/%s", dir_path,
                         entry->d_name);
                total_size += getDirectorySize(subdir_path);
            }
        }
    }

    closedir(dir);
    return total_size;
}

char *humanReadableSize(long size) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    int unitIndex = 0;
    double sizeInUnits = (double)size;

    while (sizeInUnits >= 1024 && unitIndex < 8) {
        sizeInUnits /= 1024;
        unitIndex++;
    }

    char *readableSize = malloc(20);

    sprintf(readableSize, "%.2f", sizeInUnits);

    return strcat(readableSize, units[unitIndex]);
}

void printSize(struct stat s) {
    printf("Size on disk: %s\n", humanReadableSize(s.st_size));
}

void createSymbolicLink(const char *fileName) {
    printf("Please give a name for the symlink: ");
    char linkName[1024];
    scanf("%s", linkName);
    symlink(fileName, linkName);
}

void printTimeOfLastModification(struct stat s) {
    printf("Last modified time: %s", ctime(&s.st_mtim.tv_sec));
}
