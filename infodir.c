#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


#include "infodir.h"

int folderSize;
int subFolders;
int files;
;
typedef enum {
    PROCESS, THREAD
} Mode;

void calculateSize(const char *folderName, Mode mode);


int createProcess() {
    int pid = fork();

    if (pid < 0) {
        perror(UNABLE_TO_CREATE_PROCCESS);
        exit(EXIT_FAILURE);
    }

    return pid;
}

void getFolderSize(const char *path, Mode mode) {
    char fullPath[256];
    struct dirent *dirData;
    struct stat buffer;

    DIR *directoryPointer;

    directoryPointer = opendir(path);

    int pid;

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

        if (dirData->d_type == DT_DIR) { // diretorio
            if (dirData->d_name[0] != '.') {
                strcpy(fullPath, path);
                strcat(fullPath, "/");
                strcat(fullPath, dirData->d_name);

                subFolders += 1;
                getFolderSize(fullPath, mode);
            }
        } else {
            strcpy(fullPath, path);
            strcat(fullPath, "/");
            strcat(fullPath, dirData->d_name);

            if (stat(fullPath, &buffer) < 0) { // not exists
                const unsigned int err = errno;
                fprintf(stderr, "%s: Failed in stat (file) %s: %u\n", GET_FOLDER_SIZE, fullPath, err);
                exit(EXIT_FAILURE);
            } else {
                files += 1;
                folderSize += buffer.st_size;
            }
        }
    }

    closedir(directoryPointer);
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

void calculateSize(const char *folderName, Mode mode) {
    printf("%s \n", mode == PROCESS ? METHOD_IPC : METHOD_MULTI_THREAD);

    if (!folderName) {
        perror(INVALID_FOLDER_NAME);
        exit(EXIT_FAILURE);
    }

    time_t begin = getCurrentTime();

    getFolderSize(folderName, mode);

    printf("%s: %s\n", DIRECTORY, folderName);
    printf("\n%s", DIRECTORY_CONTENT);

    printf("\n\t%s: %d ", FILES, files);
    printf("\n\t%s: %d ", SUB_FOLDERS, subFolders);
    printf("\n\t%s: %d %s", DIRECTORY_SIZE, folderSize, BYTES);
    printf("\n\t%s: %.1f Mb", SIZE_IN_MEGAS, folderSize / (1024.0 * 1024));

    printf("\n");

    time_t end = getCurrentTime();

    printf("\n%s", mode == PROCESS ? TIME_USING_IPC : TIME_USING_MULTI_THREAD);
    displayTime(BEGIN, localtime(&begin));
    printf("\n\t%s: %d %s", DURATION, (int) difftime(end, begin), SECONDS);
    displayTime(END, localtime(&end));
    printf("\n");
}

int main(int argc, char **argv) {
    // TODO remove this test line
    argc = 2;
    argv[1] = "/home/lucas/Downloads";

    calculateSize(((argc == 1) ? NULL : argv[1]), PROCESS);

    return 0;
}