#include "file_l.h"
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


void permission_translator(struct stat file, char* permissions) {
    strcpy(permissions, "");
    strcat(permissions, (file.st_mode & S_IRUSR) ? "r" : "-");
    strcat(permissions, (file.st_mode & S_IWUSR) ? "w" : "-");
    strcat(permissions, (file.st_mode & S_IXUSR) ? "x" : "-");
    strcat(permissions, (file.st_mode & S_IRGRP) ? "r" : "-");
    strcat(permissions, (file.st_mode & S_IWGRP) ? "w" : "-");
    strcat(permissions, (file.st_mode & S_IXGRP) ? "x" : "-");
    strcat(permissions, (file.st_mode & S_IROTH) ? "r" : "-");
    strcat(permissions, (file.st_mode & S_IWOTH) ? "w" : "-");
    strcat(permissions, (file.st_mode & S_IXOTH) ? "x\0" : "-\0");
}

void snap_file(char *output_path, const char *quarantine_path, char *input_path) {
    int pfd[2];
    if (pipe(pfd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t child_pid = fork();
    
    if(child_pid == -1) {
        perror("failed");
        exit(EXIT_FAILURE);
    } else if(child_pid == 0) {
        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[1]);
        char script_path[30] = "./scripts/quarantine.sh\0";
        execlp(script_path, script_path, quarantine_path, input_path, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        close(pfd[1]);
        char pipe_buff[1024];
        ssize_t num_bytes;
        while ((num_bytes = read(pfd[0], pipe_buff, sizeof(pipe_buff))) > 0) {
            // Process the data read from the pipe
            if(strncmp("SAFE", pipe_buff, 4)) {
                printf("Malicious file detected: %ld bytes: %.*s", num_bytes, (int)num_bytes, pipe_buff);
            } else {
                printf("SAFE\n");
            }
        }
        if (num_bytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        } else if (num_bytes == 0) {
            // End-of-file (EOF) reached
        }
        
        close(pfd[0]);

        int status;
        pid_t end_pid = wait(&status);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != EXIT_SUCCESS) {
                printf("Child process failed\n");
            }
        } else {
            printf("Child process terminated abnormally\n");
        }

        printf("File %s scanned successfully\n", input_path);
        printf("Grandchild Process terminated with PID %d and exit code %d\n", end_pid, WEXITSTATUS(status));


        char buff[BUFFER];

        int fr = open(input_path, O_RDONLY ,  S_IRWXU | S_IRWXG | S_IRWXO);
        if(fr == -1) {
            close(fr);
            return;
        }
        
        int fw = open(output_path, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
        if(fw == -1) {
            close(fw);
            return;
        }

        struct stat fileStat;
        char cStat[MAX_STAT];
        char permissions[11];
        permission_translator(fileStat, permissions);

        sprintf(cStat, "Last change: %s", ctime(&fileStat.st_atime));
        write(fw, cStat, sizeof(char) * strlen(cStat));

        sprintf(cStat, "File path: %s\n", input_path);
        write(fw, cStat, sizeof(char) * strlen(cStat));

        sprintf(cStat, "Size: %ld\n", fileStat.st_size);
        write(fw, cStat, sizeof(char) * strlen(cStat));

        sprintf(cStat, "Last change: %s", ctime(&fileStat.st_mtime));
        write(fw, cStat, sizeof(char) * strlen(cStat));

        sprintf(cStat, "Permissions: %s\n", permissions);
        write(fw, cStat, sizeof(char) * strlen(cStat));

        sprintf(cStat, "Inode number: %ld\n\n-------Content-------\n\n", fileStat.st_ino);
        write(fw, cStat, sizeof(char) * strlen(cStat));

        while(read(fr, buff, sizeof(buff))) {
            write(fw, buff, sizeof(char) * strlen(buff));
        }

        
        close(fr);
        close(fw);
    }
}

