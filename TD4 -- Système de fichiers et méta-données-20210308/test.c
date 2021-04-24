#define _H_INTERNALS_ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include "internals.h"

int test_capture_type(){
    struct stat *file1 = malloc(sizeof(struct file*));
    struct stat *file2 = malloc(sizeof(struct file*));
    struct stat *fileERR = malloc(sizeof(struct file*));
    struct stat *file3 = malloc(sizeof(struct file*));
    file1->st_mode = S_IFDIR;
    file2->st_mode = S_IFLNK;
    file3->st_mode = S_IFREG;
    fileERR->st_mode = 0;

    int a, b, c, d;
    a = capture_file_type(file1);
    printf("%d est un dire\n", a);
    b = capture_file_type(file2);
    printf("%d est un link\n", b);
    c = capture_file_type(file3);
    printf("%d est un reg\n", c);
    d = capture_file_type(fileERR);
    printf("%d est un pfff\n", d);

    return 0;


}