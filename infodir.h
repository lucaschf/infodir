#ifndef INFODIR_INFODIR_H
#define INFODIR_INFODIR_H

#define GET_FOLDER_SIZE "getFolderSize"
#define METHOD_IPC "Método: IPC - Interproccess Communication"
#define INVALID_FOLDER_NAME "Invalid folder name"
#define DIRECTORY "Diretório"
#define DIRECTORY_CONTENT "Conteúdo do diretório"
#define FILES "Arquivos"
#define SUB_FOLDERS "Subdiretórios"
#define UNABLE_TO_RECOVER_SIZE "Incapaz de recuperar o tamanho do diretório"
#define DIRECTORY_SIZE "Tamanho do diretório"
#define SIZE_IN_MEGAS "Tamanho do diretório em megabytes"
#define BYTES "bytes"
#define TIME_USING_IPC "Tempo usando IPC"
#define BEGIN "Início"
#define END "Término"
#define DURATION "Duração"
#define SECONDS "segundos"

time_t getCurrentTime();

/**
 * Displays a time in HH:mm:ss format
 *
 * @param message optional message to be shown.
 * @param tm the time to be shown.
 */
void displayTime(const char *message, struct tm *tm);

/**
 * Retrieves the size of the directory, the number of subdirectories and the number of files on the passed path.
 *
 * @param path the target directory path.
 * @param totalSize pointer to variable where the directory size should be stored.
 * @param subFolders pointer to variable where the sub folders count should be stored.
 * @param files pointer to variable where the files count should be stored.
 * @return EXIT_SUCCESS if successful executed, EXIT_FAILURE if something gone wrong.
 */
int getFolderSize(const char *path, int *totalSize, int *subFolders, int *files);

#endif //INFODIR_INFODIR_H
