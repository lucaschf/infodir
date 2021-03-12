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


int getFolderSize(const char *folder, int *totalSize, int *subFolders, int *files) ;

#endif //INFODIR_INFODIR_H
