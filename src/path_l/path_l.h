#ifndef PATH_L_H
#define PATH_L_H

#define MAX_PATH 1024
#define MAX_STAT 1024
#define BUFFER 4096
#define MIN_INPUT_DIRS 5
#define MAX_INPUT_DIRS 14

int dot_dir_validation(const char *path);
const char* path_sanitizer(const char* path);
void update_path(char* new_path, const char *old_path, const char *path_extender);

#endif
