#ifndef PROCESSES_L_H
#define PROCESSES_L_H

#define MAX_PATH 1024
#define MAX_STAT 1024
#define BUFFER 4096
#define MIN_INPUT_DIRS 5
#define MAX_INPUT_DIRS 14

void rec_parse(const char* output_dir_path, const char *quarantine_path, const char* input_dir_path);
void create_processes(char snap_dirs[][MAX_INPUT_DIRS], const char* output_dir, char *quarantine_path, int num_processes, int index);

#endif
