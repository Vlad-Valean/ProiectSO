#include "path_l.h"
#include<stdio.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<sys/unistd.h>
#include<sys/wait.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<errno.h>


int dot_dir_validation(const char *path) {
    return !strcmp(path, ".\0") || !strcmp(path, "..\0");
}

const char* path_sanitizer(const char* path) {
    return !strncmp(path, "./\0", 2) ? path += 2 : !strncmp(path, "../\0", 3) ? path += 3 : path;
}

void update_path(char* new_path, const char *old_path, const char *path_extender) {
    sprintf(new_path, "%s/%s", old_path, path_sanitizer(path_extender));
}

