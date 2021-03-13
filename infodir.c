#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include "infodir.h"

#define MAX_CHILDREN 4

int folderSize;
int subFolders;
int files;
time_t begin;
int children = 0;
char rootPath[250] = "";

Mode runningMode;

int createProcess() {
    int pid = fork();

    if (pid < 0) {
        perror(UNABLE_TO_CREATE_PROCCESS);
        exit(EXIT_FAILURE);
    }

    return pid;
}

void *getFolderSize(void *path) {
    char subFolderPath[256];
    struct dirent *dirData;
    struct stat buffer;

    DIR *directoryPointer;

    directoryPointer = opendir(path);

    if (!directoryPointer) {
        perror(strcat(GET_FOLDER_SIZE, ": failed to open directory"));
        exit(EXIT_FAILURE);
    }

    while ((dirData = readdir(directoryPointer))) {
        if (dirData == NULL) {
            const unsigned int err = errno;

            fprintf(stderr, "%s: Failed in readdir (%u)\n", GET_FOLDER_SIZE, err);
            exit(EXIT_FAILURE);
        }

        if (dirData->d_type == DT_DIR) { // directory
            if (dirData->d_name[0] != '.') { // valid directory

                // generates the sub folder path
                strcpy(subFolderPath, path);
                strcat(subFolderPath, "/");
                strcat(subFolderPath, dirData->d_name);

                subFolders++;

                if (strcmp(path, rootPath) == 0 && children < MAX_CHILDREN) { // can create another child
                    children++;

                    if (runningMode == PROCESS) {
                        int pid = createProcess();

                        if (pid != 0) { // child process
//                            printf("Child process %d started \n", pid);
                            waitpid(pid, NULL, 0);
//                            printf("Child process %d ended \n", pid);
                            exit(0);
                        } else
                            getFolderSize(subFolderPath);

                        continue;
                    }

                    createThread(subFolderPath);
                    continue;
                }

                getFolderSize(subFolderPath);
            }
        } else {
            strcpy(subFolderPath, path);
            strcat(subFolderPath, "/");
            strcat(subFolderPath, dirData->d_name);

            if (stat(subFolderPath, &buffer) < 0) { // not exists
                const unsigned int err = errno;
                fprintf(stderr, "%s: Failed in stat (file) %s: %u\n", GET_FOLDER_SIZE, subFolderPath, err);
                exit(EXIT_FAILURE);
            } else {
                files++;
                folderSize += buffer.st_size;
            }
        }
    }

    closedir(directoryPointer);
    return NULL;
}

void createThread(char *folderPath) {
//    int threadId = children;
//    printf("Thread %d started\n", threadId);
    pthread_t pthread;
    pthread_create(&pthread, NULL, getFolderSize, (void *) folderPath);
    pthread_join(pthread, NULL);
//    printf("Thread %d ended\n", threadId);
}

time_t getCurrentTime() {
    time_t rawTime;
    return time(&rawTime);
}

void displayTime(const char *message, struct tm *tm) {
    printf(
            "\n\t%s: %2d:%02d:%02d",
            message ? message : "",
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec
    );
}

void displayResult(const char *folderName) {
    printf("%s: %s\n", DIRECTORY, folderName);
    printf("\n%s", DIRECTORY_CONTENT);

    printf("\n\t%s: %d ", FILES, files);
    printf("\n\t%s: %d ", SUB_FOLDERS, subFolders);
    printf("\n\t%s: %d %s", DIRECTORY_SIZE, folderSize, BYTES);
//    printf("\n\t%s: %.1f MB", SIZE_IN_MEGAS, folderSize / (1024.0 * 1024));
//    printf("\n\t%s: %.1f KB", SIZE_IN_KB, folderSize / (1024.0));

    printf("\n");
    printf("\n%s", runningMode == PROCESS ? TIME_USING_IPC : TIME_USING_MULTI_THREAD);
    displayTime(BEGIN, localtime(&begin));

    time_t end = getCurrentTime();
    printf("\n\t%s: %d %s", DURATION, (int) difftime(end, begin), SECONDS);
    displayTime(END, localtime(&end));
    printf("\n\n");
}

void calculateSize(const char *folderName, Mode mode) {
    printf("%s \n", mode == PROCESS ? METHOD_IPC : METHOD_MULTI_THREAD);

    folderSize = 0;
    files = 0;
    subFolders = 0;
    children = 0;

    strcpy(rootPath, folderName);

    runningMode = mode;
    getFolderSize((char *) folderName);
    displayResult(folderName);
}

void run(char *folderName) {
    if (!folderName) {
        perror(INVALID_FOLDER_NAME);
        exit(EXIT_FAILURE);
    }

    begin = getCurrentTime();
    calculateSize(folderName, PROCESS);

    begin = getCurrentTime();
    calculateSize(folderName, THREAD);
}

int main(int argc, char **argv) {
    run((argc == 1) ? NULL : argv[1]);
    return 0;
}