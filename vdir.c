#include<stdio.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

void snap_file(char *path) {
    char snap_path[120];
    char *name;
    char *extension;
    name = strtok(path, ".");
    strcpy(snap_path, name);
    strcat(snap_path, "_snap\0");
    if((extension = strtok(NULL, "."))) {
        strcat(snap_path, ".\0");
        strcat(snap_path, extension);
    }
    printf("%d\n", open(path, O_RDONLY , S_IROTH));
    printf("%d\n", open(snap_path, O_EXCL | O_WRONLY | O_TRUNC, S_IWOTH));
}

int dot_folder_validation(const char *path) {
    return !strcmp(path, ".\0") || !strcmp(path, "..\0");
}

void update_path(char* new_path, const char *old_path, const char *path_extender) {
    strcpy(new_path, old_path);
    strcat(new_path, "/\0");
    strcat(new_path, path_extender);
}

void rec_parse(DIR *directory_obj, const char* path) {
    struct dirent *directory_content = NULL;
    DIR *child_directory_obj = NULL;
    while((directory_content = readdir(directory_obj))) {

        char child_directory_path[240];
        update_path(child_directory_path, path, directory_content->d_name);
        
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
    if(argc < 2) {
        printf("Error program call: ./vdir <directory_path>\n");
        exit(-1);
    }
    
    DIR *directory_obj = opendir(argv[1]);
    rec_parse(directory_obj, argv[1]);
    
    return 0;
}