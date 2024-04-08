#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>


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
        
            if((child_directory_obj = opendir(child_directory_path))) {
            char new_path[120];
            //daca e director
            strcpy(new_path, "/\0");
            strcat(new_path, directory_content->d_name);
            rec_parse(child_directory_obj, new_path);
            printf("\n");
            //altfel ....
            } else {
                printf("file:  %s\n", directory_content->d_name);
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