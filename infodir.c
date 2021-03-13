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

// shared memory
FolderInfo folderInfo;
Mode runningMode;
time_t begin;
int children = 0;

int createProcess() {
    int pid = fork();

    if (pid < 0) {
        perror(UNABLE_TO_CREATE_PROCCESS);
        exit(EXIT_FAILURE);
    }

    return pid;
}

void *getFolderInfo(void *path) {
    char subFolderPath[256];
    struct dirent *dirData;
    struct stat buffer;

    DIR *directoryPointer;

    directoryPointer = opendir(path);

    if (!directoryPointer) {
        perror(strcat(GET_FOLDER_INFO, UNABLE_TO_OPEN_DIRECTORY));
        exit(EXIT_FAILURE);
    }

    while ((dirData = readdir(directoryPointer))) {
        if (dirData == NULL) {
            const unsigned int err = errno;

            fprintf(stderr, "%s: %s (%u)\n", GET_FOLDER_INFO, FAILED_ON_READ, err);
            exit(EXIT_FAILURE);
        }

        if (dirData->d_type == DT_DIR) { // directory
            if (dirData->d_name[0] != '.') { // valid directory

                // generates the sub folder path by joining current path with the directory name
                strcpy(subFolderPath, path);
                strcat(subFolderPath, "/");
                strcat(subFolderPath, dirData->d_name);

                folderInfo.subFolders++;

                if (!strcmp(path, folderInfo.path) && children < MAX_CHILDREN) { // can create another child
                    children++;

                    if (runningMode == PROCESS) {
                        int pid = createProcess();

                        if (pid != 0) { // child process
//                            printf("Child process %d started \n", pid);
                            waitpid(pid, NULL, 0);
//                            printf("Child process %d ended \n", pid);
                            exit(0);
                        } else
                            getFolderInfo(subFolderPath);

                        continue;
                    }

                    createThread(subFolderPath);
                    continue;
                }

                getFolderInfo(subFolderPath);
            }

            continue;
        }

        // it's a file
        strcpy(subFolderPath, path);
        strcat(subFolderPath, "/");
        strcat(subFolderPath, dirData->d_name);

        if (stat(subFolderPath, &buffer) < 0) { // potentially not exists
            const unsigned int err = errno;
            fprintf(stderr, "%s: %s %s: %u\n", FAILED_IN_STAT_FILE, GET_FOLDER_INFO, subFolderPath, err);
            exit(EXIT_FAILURE);
        }

        folderInfo.files++;
        folderInfo.size_ += buffer.st_size;
    }

    closedir(directoryPointer);
    return NULL;
}

void createThread(char *folderPath) {
//    int threadId = children;
//    printf("Thread %d started\n", threadId);
    pthread_t pthread;
    pthread_create(&pthread, NULL, getFolderInfo, (void *) folderPath);
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

void displayResult() {
    printf("%s: %s\n", DIRECTORY, folderInfo.path);
    printf("\n%s", DIRECTORY_CONTENT);

    printf("\n\t%s: %d ", FILES, folderInfo.files);
    printf("\n\t%s: %d ", SUB_FOLDERS, folderInfo.subFolders);
    printf("\n\t%s: %lld %s", DIRECTORY_SIZE, folderInfo.size_, BYTES);
//    printf("\n\t%s: %.1f MB", SIZE_IN_MEGABYTES, folderInfo.size_ / (1024.0 * 1024));
//    printf("\n\t%s: %.1f KB", SIZE_IN_KB, folderInfo.size_ / (1024.0));

    printf("\n");
    printf("\n%s", runningMode == PROCESS ? TIME_USING_IPC : TIME_USING_MULTI_THREAD);
    displayTime(BEGIN, localtime(&begin));

    time_t end = getCurrentTime();
    printf("\n\t%s: %d %s", DURATION, (int) difftime(end, begin), SECONDS);
    displayTime(END, localtime(&end));
    printf("\n\n");
}

void getInfo(const char *path, Mode mode) {
    printf("%s \n", mode == PROCESS ? METHOD_IPC : METHOD_MULTI_THREAD);

    // resets the folder info
    folderInfo.size_ = 0;
    folderInfo.files = 0;
    folderInfo.subFolders = 0;

    children = 0;
    runningMode = mode;

    getFolderInfo((char *) path);
    displayResult();
}

void run(char *folderName) {
    if (!folderName) {
        perror(INVALID_FOLDER_NAME);
        exit(EXIT_FAILURE);
    }

    strcpy(folderInfo.path, folderName);

    begin = getCurrentTime();
    getInfo(folderName, PROCESS);

    begin = getCurrentTime();
    getInfo(folderName, THREAD);
}

int main(int argc, char **argv) {
    run((argc == 1) ? NULL : argv[1]);
    return 0;
}