#ifndef INFODIR_INFODIR_H
#define INFODIR_INFODIR_H

#define GET_FOLDER_SIZE "getFolderSize"
#define METHOD_IPC "Método: IPC - Interproccess Communication"
#define INVALID_FOLDER_NAME "Invalid folder name"
#define METHOD_MULTI_THREAD "Método: multi thread"
#define DIRECTORY "Diretório"
#define DIRECTORY_CONTENT "Conteúdo do diretório"
#define FILES "Arquivos"
#define SUB_FOLDERS "Subdiretórios"
#define UNABLE_TO_RECOVER_SIZE "Incapaz de recuperar o tamanho do diretório"
#define DIRECTORY_SIZE "Tamanho do diretório"
#define SIZE_IN_MEGAS "Tamanho do diretório em megabytes"
#define SIZE_IN_KB "Tamanho do diretório em Kbytes"
#define BYTES "bytes"
#define TIME_USING_IPC "Tempo usando IPC"
#define TIME_USING_MULTI_THREAD "Tempo usando multi thread"
#define BEGIN "Início"
#define END "Término"
#define DURATION "Duração"
#define SECONDS "segundos"
#define UNABLE_TO_CREATE_PROCCESS "Incapaz de criar processo"

time_t getCurrentTime();

/**
 * Tries to create a process, if fails, shows a error message and exit program.
 *
 * @return the created pid
 */
int createProcess();

/**
 * Displays a time in HH:mm:ss format
 *
 * @param message optional message to be shown.
 * @param tm the time to be shown.
 */
void displayTime(const char *message, struct tm *tm);
#endif //INFODIR_INFODIR_H
