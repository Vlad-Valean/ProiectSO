#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>



int main(int argc, char *argv)  {
    char *directory_path;
    if(argc > 1) {
        strcpy(directory_path, argv[1]);
    } else { 
        perror("program call: ./vdir <directory_path>\n");
        exit(-1);
    }
    
    DIR *directory_obj = opendir(directory_path);



    return 0;
}