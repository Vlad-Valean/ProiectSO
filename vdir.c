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


void rec_parse(DIR *directory_obj, const char* path) {
    struct dirent *directory_content = NULL;
    DIR *child_directory_obj = NULL;
    printf("\nFOLDER:  %s:\n\n", path);
    while((directory_content = readdir(directory_obj))) {

        char child_directory_path[120] = "\0";
        strcpy(child_directory_path, path);
        strcat(child_directory_path, "/\0");
        strcat(child_directory_path, directory_content->d_name);
        
        if(!strcmp(directory_content->d_name, ".\0") || !strcmp(directory_content->d_name, "..\0")) {
        } else {
            char new_path[120];
            strcpy(new_path, "/\0");
            strcat(new_path, directory_content->d_name);
            if((child_directory_obj = opendir(child_directory_path))) {

            rec_parse(child_directory_obj, new_path);
            printf("\n");
            //altfel ....
            } else {
                printf("file:  %s\n", new_path);
                snap_file(new_path);
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