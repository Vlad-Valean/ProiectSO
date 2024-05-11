#include "main.h"
#include "./processes_l/processes_l.h"
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

int main(const int argc, const char **argv)  {
    
    if(argc < MIN_INPUT_DIRS || argc > MAX_INPUT_DIRS ) { 
        printf("Usage: ./snapdir -o <output_directory_path> -s <quarantine_directory_path> <directories_to_be_snapped ...>\n");
        exit(-1);
    }
    
    char odir_path[MAX_PATH];
    char qdir_path[MAX_PATH];
    char sdir_path[MAX_PATH][MAX_INPUT_DIRS];
    int num_dirs = 0;
    
  for (int i = 1; i < argc; i++) {
        if (strcmp("-o", argv[i]) == 0) {
            if (i + 1 < argc) {
                strcpy(odir_path, argv[i + 1]);
                i++;
            } else {
                printf("Error: Missing output directory path.\n");
                exit(-1);
            }
        } else if (strcmp("-s", argv[i]) == 0) {
            strcpy(qdir_path, argv[i + 1]);
            i++;
        } else {
            strcpy(sdir_path[num_dirs], argv[i]);
            num_dirs++;
        }
    }

    if (num_dirs == 0) {
        printf("Error: No directories to be snapped provided.\n");
        exit(-1);
    }
 
    mkdir(odir_path, S_IRWXU);
    mkdir(qdir_path, S_IRWXU);
    
    create_processes(sdir_path, odir_path, qdir_path, argc - MIN_INPUT_DIRS, 0);   
    return 0;
}