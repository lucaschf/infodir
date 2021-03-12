#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#include "infodir.h"

int getFolderSize(const char *path, int *totalSize, int *subFolders, int *files) {
    char fullPath[256];
    struct dirent *dirData;
    struct stat buffer;
    int exists;
    DIR *directoryPointer;

    int result = EXIT_SUCCESS;

    directoryPointer = opendir(path);

    if (!directoryPointer) {
        perror(strcat(GET_FOLDER_SIZE, ": failed to open directory"));
        return EXIT_FAILURE;
    }

    while ((dirData = readdir(directoryPointer))) {
        if (dirData == NULL) {
            const unsigned int err = errno;

            fprintf(stderr, "%s: Failed in readdir (%u)\n", GET_FOLDER_SIZE, err);
            result = EXIT_FAILURE;

            continue;
        }

        if (dirData->d_type == DT_DIR) {
            if (dirData->d_name[0] != '.') {
                strcpy(fullPath, path);
                strcat(fullPath, "/");
                strcat(fullPath, dirData->d_name);

                if (getFolderSize(fullPath, totalSize, subFolders, files) == EXIT_FAILURE)
                    result = EXIT_FAILURE;

                *subFolders += 1;
            }
        } else {
            strcpy(fullPath, path);
            strcat(fullPath, "/");
            strcat(fullPath, dirData->d_name);

            exists = stat(fullPath, &buffer);

            if (exists < 0) {
                const unsigned int err = errno;
                fprintf(stderr, "%s: Failed in stat (file) %s: %u\n", GET_FOLDER_SIZE, fullPath, err);
                result = EXIT_FAILURE;

                continue;
            } else {
                (*files) += 1;
                (*totalSize) += buffer.st_size;
            }
        }
    }

    closedir(directoryPointer);

    return result;
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

int calculateSize(const char *folderName) {

    int subFolders = 0;
    int files = 0;
    int folderSize;

    if (!folderName) {
        perror(INVALID_FOLDER_NAME);
        return EXIT_FAILURE;
    }

    time_t begin = getCurrentTime();

    if (getFolderSize(folderName, &folderSize, &subFolders, &files) == EXIT_FAILURE)
        printf(UNABLE_TO_RECOVER_SIZE);
    else {
        printf("%s: %s\n", DIRECTORY, folderName);
        printf("\n%s", DIRECTORY_CONTENT);

        printf("\n\t%s: %d ", FILES, files);
        printf("\n\t%s: %d ", SUB_FOLDERS, subFolders);
        printf("\n\t%s: %d %s", DIRECTORY_SIZE, folderSize, BYTES);
        printf("\n\t%s: %.1f Mb", SIZE_IN_MEGAS, folderSize / (1024.0 * 1024));

        printf("\n");
    }

    time_t end = getCurrentTime();

    printf("\n%s", TIME_USING_IPC);
    displayTime(BEGIN, localtime(&begin));
    printf("\n\t%s: %d %s", DURATION, (int) difftime(end, begin), SECONDS);
    displayTime(END, localtime(&end));

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    printf("%s \n", METHOD_IPC);

    return calculateSize(((argc == 1) ? NULL : argv[1]));
}