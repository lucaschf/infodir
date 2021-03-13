#ifndef INFODIR_INFODIR_H
#define INFODIR_INFODIR_H

#define GET_FOLDER_INFO "getFolderInfo"
#define METHOD_IPC "Método: IPC - Interprocess Communication"
#define INVALID_FOLDER_NAME "Invalid folder name"
#define METHOD_MULTI_THREAD "Método: multi thread"
#define FAILED_IN_STAT_FILE "Falha na checagem de stat (arquivo)"
#define DIRECTORY "Diretório"
#define DIRECTORY_CONTENT "Conteúdo do diretório"
#define FILES "Arquivos"
#define SUB_FOLDERS "Subdiretórios"
#define UNABLE_TO_OPEN_DIRECTORY "Incapaz de abrir diretorio"
#define FAILED_ON_READ "Falha na leitura"
#define DIRECTORY_SIZE "Tamanho do diretório"
#define SIZE_IN_MEGABYTES "Tamanho do diretório em Megabytes"
#define SIZE_IN_KB "Tamanho do diretório em Kbytes"
#define BYTES "bytes"
#define TIME_USING_IPC "Tempo usando IPC"
#define TIME_USING_MULTI_THREAD "Tempo usando multi thread"
#define BEGIN "Início"
#define END "Término"
#define DURATION "Duração"
#define SECONDS "segundos"
#define UNABLE_TO_CREATE_PROCCESS "Incapaz de criar processo"

// Represents the available operation modes
typedef enum {
    PROCESS, THREAD
} Mode;

// Represents the directory data we are looking for
typedef struct {
    char path[250];
    long long int size_;
    int subFolders;
    int files;
} FolderInfo;

/**
 * Gets current system time
 */
time_t getCurrentTime();

void createThread(char *folderPath);

/**
 * Tries to create a process, if fails, shows a error message and exit program.
 *
 * @return the created pid
 */
int createProcess();

/**
 * Displays a time in hh:mm:ss format
 *
 * @param message optional message to be shown.
 * @param tm the time to be shown.
 */
void displayTime(const char *message, struct tm *tm);

/**
 * Retrieves information from a directory based on its path.
 *
 * @param path full directory path
 * @param mode the operation mode
 */
void getInfo(const char *path, Mode mode);

#endif //INFODIR_INFODIR_H
