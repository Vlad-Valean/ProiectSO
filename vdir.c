#include<stdio.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<sys/unistd.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

void snap_file(char *path) {
    char snap_path[120];
    char *name;
    char *extension;
    name = strtok(path, ".");
    printf("%s\n", name);
    strcpy(snap_path, name);
    strcat(snap_path, "_snap\0");
    if((extension = strtok(NULL, "."))) {
        strcat(snap_path, ".\0");
        strcat(snap_path, extension);
    }
    // int fr = open(path, O_RDONLY , S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH );
    // int fw = open(snap_path, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, S_IWUSR | S_IWGRP | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH);
    // printf("%d\n", fr);
    // printf("%d\n", fw);
    // perror("open");
    // close(fr);
    // close(fw);
}

int dot_folder_validation(const char *path) {
    return !strcmp(path, ".\0") || !strcmp(path, "..\0");
}

void update_path(char* new_path, const char *old_path, const char *path_extender) {
    strcpy(new_path, old_path);
    strcat(new_path, "/\0");
    strcat(new_path, path_extender);
}

void rec_parse(const char* odir_path, const char* sdir_path) {
    DIR *directory_obj = opendir(odir_path);
    struct dirent *directory_content = NULL;
    DIR *child_directory_obj = NULL;
    while((directory_content = readdir(directory_obj))) {

        char child_directory_path[240];
        update_path(child_directory_path, odir_path, directory_content->d_name);
        
        if(dot_folder_validation(directory_content->d_name)) {
        } else {
            
            printf("%s\n", child_directory_path);
            if((child_directory_obj = opendir(child_directory_path))) {
            rec_parse(child_directory_obj, child_directory_path);
            
            } else {
                snap_file(child_directory_path);
            }
        } 

        
    }
    closedir(directory_obj);
}

int main(const int argc, const char **argv)  {
    if(argc < 2) { // 3
        printf("Error program call: ./vdir <output_directory_path> <directories_to_be_snapped\n");
        exit(-1);
    }
    char odir_path[120];
    char sdir_path[120];
    strcpy(odir_path, argv[1]);
    strcpy(sdir_path, argv[2]);
    mkdir(odir_path, S_IRWXU);
    rec_parse(odir_path, sdir_path);
    
    return 0;
}