#ifndef FILE_L_H
#define FILE_L_H

#include <sys/stat.h>

#define MAX_PATH 1024
#define MAX_STAT 1024
#define BUFFER 4096
#define MIN_INPUT_DIRS 5
#define MAX_INPUT_DIRS 14

void permission_translator(struct stat file, char* permissions);
void snap_file(char *output_path, const char *quarantine_path, char *input_path);

#endif
