#include<stdio.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<sys/unistd.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#define MAX_PATH 1028
#define MAX_INPUT_DIRS 10
#define MIN_INPUT_DIRS 2

void snap_file(char *output_path, char *input_path) {
    int fr = open(input_path, O_RDONLY , S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH );
    int fw = open(output_path, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, S_IWUSR | S_IWGRP | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH);
    printf("%d\n", fr);
    printf("%d\n", fw);
    perror("open");
    close(fr);
    close(fw);
}

int dot_dir_validation(const char *path) {
    return !strcmp(path, ".\0") || !strcmp(path, "..\0");
}

void update_path(char* new_path, const char *old_path, const char *path_extender) {
    char update_path_extender[MAX_PATH];
    if(!strncmp(path_extender, "./\0", 2)) {
        path_extender += 2;
    } else if(!strncmp(path_extender, "../\0", 3)) {
        path_extender += 3;
    }

    strcpy(update_path_extender, path_extender);
    strcpy(new_path, old_path);
    strcat(new_path, "/\0");
    strcat(new_path, update_path_extender);
}

void rec_parse(const char* output_dir_path, const char* input_dir_path) {
    DIR *input_dir_obj = opendir(input_dir_path); // input 
    struct dirent *input_dir_content = NULL; // input
    DIR *child_input_dir_obj = NULL; // input rec
    while((input_dir_content = readdir(input_dir_obj))) {

        char child_input_dir_path[MAX_PATH];
        char child_output_dir_path[MAX_PATH];
     
        if(dot_dir_validation(input_dir_content->d_name)) {
        } else {
            update_path(child_input_dir_path, input_dir_path, input_dir_content->d_name); //update input path
            update_path(child_output_dir_path, output_dir_path, child_input_dir_path); //generate output(snap) path
            if((child_input_dir_obj = opendir(child_input_dir_path))) {
            mkdir(child_output_dir_path, S_IRWXU);
            rec_parse(output_dir_path, child_input_dir_path);
            } else {
                snap_file(child_output_dir_path, child_input_dir_path);
            }
        } 

    }
    closedir(input_dir_obj);
}

int main(const int argc, const char **argv)  {
    if(argc < MIN_INPUT_DIRS || argc > MAX_INPUT_DIRS ) { // 3
        printf("Error program call: ./vdir <output_directory_path> <directories_to_be_snapped ...>\n");
        exit(-1);
    }
    char odir_path[MAX_PATH];
    char sdir_path[MAX_PATH][MAX_INPUT_DIRS];
    strcpy(odir_path, argv[1]);
    for(int i  = MIN_INPUT_DIRS; i < argc; i++) {
        strcpy(sdir_path[i - MIN_INPUT_DIRS], argv[i]);
    }
    mkdir(odir_path, S_IRWXU);
    perror("mesaj");
    
    for(int i = 0; i < argc - MIN_INPUT_DIRS; i++) {
        // printf("%s\n", sdir_path[i]); 
        rec_parse(odir_path, sdir_path[i]);
        // verifica daca sunt fisiere daca sunt fisiere returneaza mesaj eroare sau daca exista
        // ruleaza rec_parse pe procese diferite refactorizeaza codul de la recparse
    }
    
    return 0;
}