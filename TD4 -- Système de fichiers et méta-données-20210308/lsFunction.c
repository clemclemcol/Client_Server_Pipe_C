#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>


char *print_type(struct stat *file_info){
    char* type = malloc (4*sizeof(char));
    switch (file_info->st_mode & S_IFMT) {
       case S_IFBLK:  sprintf(type, "devc");            break;
       case S_IFCHR:  sprintf(type, "devc");            break;
       case S_IFDIR:  sprintf(type, "dir");             break;
       case S_IFIFO:  sprintf(type, "FIFO");            break;
       case S_IFLNK:  sprintf(type, "link");            break;
       case S_IFREG:  sprintf(type, "file");            break;
       case S_IFSOCK: sprintf(type, "sock");            break;
       default:       sprintf(type, "unkn");            break;
    }
    return type;
}

char *print_perm (struct stat *file_info){
    // show the access permission 
    char *perm = malloc(9*sizeof(char));
    sprintf(perm, "%c%c%c%c%c%c%c%c%c", 
        (S_IRUSR  & file_info->st_mode ) ? 'r' : '-',         //user
        (S_IWUSR  & file_info->st_mode ) ? 'w' : '-', 
        (S_IXUSR  & file_info->st_mode ) ? 'x' : '-', 

        (S_IRGRP  & file_info->st_mode ) ? 'r' : '-',        //group
        (S_IWGRP  & file_info->st_mode ) ? 'w' : '-', 
        (S_IXGRP  & file_info->st_mode ) ? 'x' : '-', 

        (S_IROTH  & file_info->st_mode ) ? 'r' : '-',        //other
        (S_IWOTH  & file_info->st_mode ) ? 'w' : '-', 
        (S_IXOTH  & file_info->st_mode ) ? 'x' : '-');
    
    return perm;
}

char* print_owner(const struct stat *file_info){
    struct passwd *pwd;
    pwd = getpwuid(file_info->st_uid);
    return pwd->pw_name;
}

char* print_file_infos (char *path){
    struct stat *file_info = malloc(sizeof(struct stat));
    lstat(path, file_info);

    char *type = print_type(file_info);   //show type
    char *owner = print_owner(file_info);// show owner
    char *perm = print_perm(file_info);  // show access permissions and file size
    char *file_desc = calloc(strlen(type)+strlen(owner)+strlen(perm)+3+10,1);
   
    sprintf(file_desc, "%s %s %s %ld\n", type, perm, owner, file_info->st_size);
   
   free(type);
   free(owner);
   free(perm);
   return file_desc;
}

int is_dir (const char* path){
    struct stat st;
    lstat(path,&st);
    return(S_ISDIR(st.st_mode));
}


void ls (char *path){
    DIR *dp;
    struct dirent *entry;

    if((dp = opendir(path)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", path);
        return;
    }
    chdir (path);
    while ((entry = readdir(dp)!= NULL)){   ///pbm
        fprintf(stdout, "  %s\n", entry->d_name);
    }
    fprintf(stdout, "\n");
    closedir(dp);
}

void ls_moins_l(char *path){
    DIR* dir;
    struct dirent *entry;

    if(!is_dir(path)){
        fprintf(stderr, "%s is not a directory\n", path);
    }
    dir = opendir(path);

    while (entry =readdir(dir)){
        if(entry->d_name[0] !='.'){
            char *entry_path_name = malloc(strlen(path)+strlen(entry->d_name)+1);
            char *file_desc;
            if(!entry_path_name){
                fprintf(stderr, "allocation failed");
            }
            sprintf(entry_path_name, "%s/%s", path, entry->d_name);
            file_desc = print_file_infos(entry_path_name);
            fprintf(stdout, "%s %s\n", file_desc, entry->d_name);
            free(entry_path_name);
            free(file_desc);
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{
    if (argc>1){
        print_file_infos(argv[1]);
    }
    else {
        fprintf(stdout,"%s\n", print_file_infos("."));
        ls(".");
        ls_moins_l(".");
        }
    return 0;
}