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

#define MAX_PATH 1024
#define MAX_STAT 1024
#define BUFFER 4096
#define MIN_INPUT_DIRS 5
#define MAX_INPUT_DIRS 14


//file functions
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

void snap_file(char *output_path, char *input_path) {
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

// path functions
int dot_dir_validation(const char *path) {
    return !strcmp(path, ".\0") || !strcmp(path, "..\0");
}

const char* path_sanitizer(const char* path) {
    return !strncmp(path, "./\0", 2) ? path += 2 : !strncmp(path, "../\0", 3) ? path += 3 : path;
}

void update_path(char* new_path, const char *old_path, const char *path_extender) {
    sprintf(new_path, "%s/%s", old_path, path_sanitizer(path_extender));
}


// main functions
void rec_parse(const char* output_dir_path, const char* input_dir_path) {
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
                rec_parse(output_dir_path, child_input_dir_path);
            } else {
                snap_file(child_output_dir_path, child_input_dir_path);
            }
        } 

    }
    closedir(input_dir_obj);
}

void create_processes(char snap_dirs[][MAX_INPUT_DIRS], const char* output_dir, int num_processes, int index) {
    
    if(index >= num_processes)
        return;

    int pid = fork();

    if(pid == -1) {
        printf("process %d ", index);
        perror("failed");
        exit(EXIT_FAILURE);
    } else if(pid == 0) {
        rec_parse(output_dir, snap_dirs[index]);        
    } else {
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != EXIT_SUCCESS) {
                printf("Child process failed for directory: %s\n", snap_dirs[index]);
            }
        } else {
            printf("Child process terminated abnormally for directory: %s\n", snap_dirs[index]);
        }
        create_processes(snap_dirs, output_dir, num_processes, ++index);
    }
}

int main(const int argc, const char **argv)  {
    
    if(argc < MIN_INPUT_DIRS || argc > MAX_INPUT_DIRS ) { 
        printf("Usage: ./vdir -o <output_directory_path> -s <quarantine_directory_path> <directories_to_be_snapped ...>\n");
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
    
    create_processes(sdir_path, odir_path, argc - MIN_INPUT_DIRS, 0);   
    return 0;
}