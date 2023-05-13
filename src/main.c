#include "files/files.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, const char *argv[]) {
    int i;
    int pid;
    int status;
    FILE *output = fopen("grades.txt", "w");
    int pipefd[2];
    char buf[256];

    for (i = 1; i < argc; i++) {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(1);
        }

        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        else if (pid == 0) {
            close(pipefd[READ_END]);

            struct stat s;
            lstat(argv[i], &s);
            switch (s.st_mode & S_IFMT) {
                case S_IFLNK: {
                    char *operations = printMenu(SYMLINK, argv[i]);
                    write(pipefd[WRITE_END], operations, strlen(operations));
                    break;
                }
                case S_IFDIR: {
                    char *operations = printMenu(DIRECTORY, argv[i]);
                    write(pipefd[WRITE_END], operations, strlen(operations));
                    break;
                }
                case S_IFREG: {
                    char *operations = printMenu(REGULAR, argv[i]);
                    write(pipefd[WRITE_END], operations, strlen(operations));
                    break;
                }
            }
            exit(0);
        }
        else {
            struct stat s;
            lstat(argv[i], &s);
            close(pipefd[WRITE_END]);
            size_t nbytes = read(pipefd[READ_END], buf, sizeof(buf));
            buf[nbytes] = '\0';
            switch (s.st_mode & S_IFMT) {
                case S_IFLNK: {
                    runOperations(SYMLINK, argv[i], buf);
                    break;
                }
                case S_IFDIR: {
                    runOperations(DIRECTORY, argv[i], buf);
                    break;
                }
                case S_IFREG: {
                    runOperations(REGULAR, argv[i], buf);
                    break;
                }
            }
        }

        int pid2 = fork();
        if (pid2 < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid2 == 0) {
            struct stat s;
            lstat(argv[i], &s);
            switch (s.st_mode & S_IFMT) {
                case S_IFLNK: {
                    execlp("chmod", "chmod", "760", argv[i], NULL);
                    exit(0);
                }
                case S_IFDIR: {
                    execlp("sh", "sh", "dir-file.sh", argv[i], NULL);
                    exit(0);
                }
                case S_IFREG: {
                    int len = strlen(argv[i]);
                    if (len >= 2 && strcmp(&argv[i][len - 2], ".c") == 0) {
                        char command[256] = "sh score.sh ";
                        strcat(command, argv[i]);
                        char buffer[1024];

                        FILE *fp = popen(command, "r");

                        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                            fprintf(output, "%s", buffer);
                        }

                        pclose(fp);
                    } else {
                        char command[256] = "wc -l < ";
                        strcat(command, argv[i]);
                        char buffer[1024];

                        FILE *fp = popen(command, "r");

                        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                            fprintf(output, "lines in %s: %s", argv[i], buffer);
                        }

                        pclose(fp);
                    }
                    exit(0);
                }
            }
        } else {
            pid = wait(&status);

            if (WIFEXITED(status)) {
                printf("The process with PID %d has ended with the exit code %d.\n", pid, WEXITSTATUS(status));
            }
        }
    }

    for (i = 1; i < argc; i++) {
        pid = wait(&status);

        if (WIFEXITED(status)) {
            printf("The process with PID %d has ended with the exit code %d.\n", pid, WEXITSTATUS(status));
        }
    }

    return 0;
}
