#include "internals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <bits/stat.h>
#include <libgen.h>
#include <sys/fsuid.h>
#include <errno.h>

int capture_racine(const char *path, struct file *racine)
{
    if (path == NULL)
        return 1;
    struct stat *stt = malloc(sizeof(struct stat));
        if (stt == NULL) return 1;

    lstat(path, stt);

    char *name = malloc(NAME_MAX * sizeof(char));
        if (name == NULL) return 1;

    strcpy(name, path);

    if (stt->st_mode == 0){
        free(name);
        free((struct stat*)stt);
        return 1;}

    if ((stt->st_mode & S_IFMT) == S_IFDIR)
    {
        racine->type = 1;
        strcpy(racine->name, name);
        racine->attribute.child = malloc(sizeof(struct file*));
        if (racine->attribute.child == NULL) return 1;
        racine->attribute.child = NULL;
        browse_directory(path, &racine);
    }
    else if ((stt->st_mode & S_IFMT) == S_IFLNK)
    {
        racine->type = 2;
        char *basenL = basename(name);
        char * buff = malloc(NAME_MAX * sizeof(char));
        if (buff == NULL) return 1;

        strcpy(racine->name, basenL);
        int nb = readlink(path, buff, NAME_MAX);
        strcpy(racine->attribute.target, buff);
        if (nb == -1)
        {
            free((struct stat*)stt);
            free(name);
            free(buff);

            printf("Erreur link.\n");
            return 1;
        }
        free(buff);
    }
    else if ((stt->st_mode & S_IFMT) == S_IFREG)
    {
        racine->type = 0;
        char *basenF = basename(name);
        strcpy(racine->name, basenF);
        racine->attribute.size = stt->st_size;
    }
    else
    {
        printf("Unkown type. \n");
        free(name);
        free((struct stat*)stt);
        return 1;
    }
    racine->user_id = stt->st_uid;
    racine->mode = stt->st_mode;
    racine->next = NULL;

    free(name);
    free(stt);
    return 0;
}

/**
 * Fill a file data structure following the target metadata
 *
 * Only files which are regular, directory or link files should be captured.
 *
 * \param[in]       path        File path
 * \param[inout]    parent      Parent directory
 * \return                      0 on success,
 *                              1 on failure
 */
int capture_file(const char *path, struct file *parent)
{
    if (path == NULL || parent == NULL)
        return 1;

    struct stat *stt = malloc(sizeof(struct stat));
        if (stt == NULL) return 1;

    struct file *enfant = malloc(sizeof(struct file));
        if (enfant == NULL) return 1;

    lstat(path, stt);
   

    char *name = malloc(sizeof(char));
        if (name == NULL) return 1;

    sprintf(name,"%s" ,path);

    if (stt->st_mode == 0)
    {
        free(name);
        free((struct stat*)stt);
        free((struct file*)enfant);
        return 1;
    }

    enfant->user_id = stt->st_uid;
    enfant->mode = stt->st_mode;

    if (S_ISREG(stt->st_mode))
    {
        enfant->type = 0;
        char *basenF = basename(name);
        strcpy(enfant->name, basenF);
        enfant->attribute.size = stt->st_size;

    }

    else if (S_ISDIR(stt->st_mode))
    {
        enfant->type = 1;
        char *basenD = basename(name);
        strcpy(enfant->name, basenD);
        enfant->attribute.child = NULL;
        browse_directory(path, &enfant);
    }

    else if (S_ISLNK(stt->st_mode))
    {
        enfant->type = 2;
        char *basenL = basename(name);
        strcpy(enfant->name, basenL);
        char *buff = malloc(NAME_MAX*sizeof(char));
        if (buff == NULL) return 1;

        int nb = readlink(path, buff, NAME_MAX);
        strcpy(enfant->attribute.target, buff);
        if (nb == -1)
        {
            free(buff);
            free(name);
            free((struct stat*)stt);
            free((struct file*)enfant);
            printf("Erreur link.\n");
            return 1;
        }
        free(buff);
    }
    else
    {
        free(name);
        free((struct stat*)stt);
        free((struct file*)enfant);
        printf("Unkown type. \n");
        return 1;
    }
    if (parent->attribute.child == NULL)
    {
        parent->attribute.child = enfant;
    }
    else
    {
        enfant->next = parent->attribute.child;
        parent->attribute.child = enfant;
    }
    free(name);
    free((struct stat*)stt);
    return 0;
}

/**
 * Browse a directory to capture its files
 *
 * If \a current is NULL, we need to allocate and initialize it.
 * It will be the caller responsability to free it.
 *
 * \param[in]       path        Directory path
 * \param[inout]    current     Directory data structure
 * \return                      0 on success,
 *                              1 on failure
 */
int browse_directory(const char *path, struct file **current)
{
    DIR *currentDir;
    struct dirent *entry;
    currentDir = opendir(path);
    if ((currentDir) == NULL)
    {
        fprintf(stderr, "Cannot open directory: %s\n", path);
        return 1;
    }
    entry = readdir(currentDir);
    char *newpath = malloc(NAME_MAX * sizeof(char));
        if (newpath == NULL) return 1;

    while ((entry != NULL))
    {
        if (entry->d_name[0] != '.')
        {
            
            strcpy(newpath, path);
            strcat(newpath, "/");
            strcat(newpath, entry->d_name);
            sprintf(newpath, "%s/%s", path, entry->d_name);

            if (!newpath)
            {
                closedir(currentDir);
                free(entry);
                free(newpath);
                fprintf(stderr, " Allocation failed.");
                
                return 1;
            }
            capture_file(newpath, (*current));
        }
        entry = readdir(currentDir);
    }
    free(newpath);
    closedir(currentDir);
    

    return 0;
}

/**
 * Create a 'phantom' regular file
 *
 * The contents of this file does not matter, but its size must match
 * the original.
 *
 * \param[in]       path        Directory path
 * \param[in]       regular     'Phantom' regular file
 * \return                      0 on success,
 *                              1 on failure
 */
int create_regular(const char *path, const struct file *regular)
{
    if (path == NULL || regular->attribute.size == 0)
        return 1;

    struct stat *st = malloc(sizeof(struct stat)); 
        if (st == NULL) return 1;

    char *newpath = malloc(NAME_MAX* sizeof(char));
        if (newpath == NULL) return 1;

    sprintf(newpath, "%s/%s", path, regular->name);
    stat(newpath, st);

    FILE *phant_reg;
    phant_reg = fopen(newpath, "a");
    if (phant_reg == NULL){
        free(st);
        fclose(phant_reg);
        free(newpath);
        return 1;
    }

    off_t taille = st->st_size;

    while (taille < regular->attribute.size)
    {
        fprintf(phant_reg, "a");
        taille++;
    }
    if (taille > regular->attribute.size)
    {
        free(st);
        fclose(phant_reg);
        free(newpath);
        return 1;
    }
    chmod(newpath, regular->mode); // changer permission
    chown(newpath,regular->user_id, -1);    // set user id
    fclose(phant_reg);

    return 0;
}

/**
 * Create a 'phantom' directory
 *
 * \param[in]       path        Parent directory path
 * \param[in]       directory   'Phantom' directory
 * \return                      0 on success,
 *                              1 on failure
 */
int create_directory(const char *path, const struct file *directory)
{
    if (path == NULL || directory == NULL)
    {
        return 1;
    }

    mkdir(path, directory->mode);

    if (directory->attribute.child == NULL)
    {
        printf("Directory sans sous fichier\n");
    
        return 0;
    }
    else
    {
        DIR *phant_dir = opendir(path);
        if ((phant_dir) == NULL)
        {
            fprintf(stderr, "cannot open directory: %s\n", path);
            return 1;
        }
       
        closedir(phant_dir);
    }

    return 0;
}

/**
 * Create a 'phantom' link
 *
 * \param[in]       path        Directory path
 * \param[in]       link        'Phantom' link
 * \return                      0 on success,
 *                              1 on failure
 */
int create_link(const char *path, const struct file *lin)
{
    if (path == NULL || lin->attribute.target == NULL)
        return 1;

    char *newpath = malloc(NAME_MAX * sizeof(char));
        if (newpath == NULL) return 1;

    sprintf(newpath, "%s/%s", path, lin->name);
    symlink(lin->attribute.target, newpath);

    free(newpath);
    
    return 0;
}
