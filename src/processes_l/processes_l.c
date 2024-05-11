#include "processes_l.h"
#include "../file_l/file_l.h"
#include "../path_l/path_l.h"
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



void rec_parse(const char* output_dir_path, const char *quarantine_path, const char* input_dir_path) {
    int malicious = 0;
    DIR *input_dir_obj = opendir(input_dir_path); 
    struct dirent *input_dir_content = NULL;
    DIR *child_input_dir_obj = NULL;
    char creat_oi_path[MAX_PATH];
    update_path(creat_oi_path, output_dir_path, input_dir_path);
    mkdir(creat_oi_path, S_IRWXU);
    while((input_dir_content = readdir(input_dir_obj))) {

        char child_input_dir_path[MAX_PATH];
        char child_output_dir_path[MAX_PATH];
     
        if(dot_dir_validation(input_dir_content->d_name)) {
        } else {
            update_path(child_input_dir_path, input_dir_path, input_dir_content->d_name); //update input path
            update_path(child_output_dir_path, output_dir_path, child_input_dir_path); //generate output(snap) path
            if((child_input_dir_obj = opendir(child_input_dir_path))) {
                rec_parse(output_dir_path, quarantine_path, child_input_dir_path);
            } else {
                snap_file(child_output_dir_path, quarantine_path, child_input_dir_path);
            }
        } 

    }
    closedir(input_dir_obj);
}

void create_processes(char snap_dirs[][MAX_INPUT_DIRS], const char* output_dir, char *quarantine_path, int num_processes, int index) {
    if(index >= num_processes)
        return;

    int pid = fork();

    if(pid == -1) {
        printf("process %d ", index);
        perror("failed");
        exit(EXIT_FAILURE);
    } else if(pid == 0) {
        rec_parse(output_dir, quarantine_path, snap_dirs[index]);
        exit(0);        
    } else {
        int status;
        pid_t end_pid = wait(&status);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != EXIT_SUCCESS) {
                printf("Child process failed for directory: %s\n", snap_dirs[index]);
            }
        } else {
            printf("Child process terminated abnormally for directory: %s\n", snap_dirs[index]);
        }
        printf("Snapshot for Directory %s created successfully\n", snap_dirs[index]);
        printf("Child Process %d terminated with PID %d and exit code %d\n", index, end_pid, WEXITSTATUS(status));
        create_processes(snap_dirs, output_dir, quarantine_path, num_processes, ++index);
    }
}
