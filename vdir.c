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

void rec_parse(const char* output_dir_path, const char* input_dir_path) {
    DIR *output_dir_obj = opendir(output_dir_path); // output
    DIR *input_dir_obj = opendir(input_dir_path); // input 
    struct dirent *input_dir_content = NULL; // input
    DIR *child_input_dir_obj = NULL; // input rec
    while((input_dir_content = readdir(input_dir_obj))) {

        char child_input_dir_path[240];
        char child_output_dir_path[240];
        
        update_path(child_input_dir_path, input_dir_path, input_dir_content->d_name); //update input path
        update_path(child_output_dir_path, output_dir_path, child_input_dir_path); //generate output(snap) path

        if(dot_folder_validation(input_dir_content->d_name)) {
        } else {
            
            printf("%s\n", child_input_dir_path);
            if((child_input_dir_obj = opendir(child_input_dir_path))) {
            rec_parse(output_dir_path, child_input_dir_path);
            
            } else {
                snap_file(child_input_dir_path);
            }
        } 

    }
    closedir(input_dir_obj);
}

int main(const int argc, const char **argv)  {
    if(argc < 2 || argc > 10 ) { // 3
        printf("Error program call: ./vdir <output_directory_path> <directories_to_be_snapped ...>\n");
        exit(-1);
    }
    char odir_path[120];
    char sdir_path[120][10];
    strcpy(odir_path, argv[1]);
    for(int i  = 2; i < argc; i++) {
        strcpy(sdir_path[i-2], argv[i]);
    }
    mkdir(odir_path, S_IRWXU);
    perror("mesaj");
    
    for(int i = 0; i < argc - 2; i++) {
        printf("%s\n", sdir_path[i]); 
        // rec_parse(odir_path, sdir_path);
        // verifica daca sunt fisiere daca sunt fisiere returneaza mesaj eroare sau daca exista
        // ruleaza rec_parse pe procese diferite refactorizeaza codul de la recparse
    }
    
    
    return 0;
}