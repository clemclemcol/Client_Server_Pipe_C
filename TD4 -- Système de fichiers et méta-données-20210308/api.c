#define _H_API_
#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "internals.h"


/**
 * Capture a tree hierarchy in a phantom data structure
 *
 * It will be the caller responsability to free \a phantom.
 *
 * \param[in]       path        Tree path
 * \param[in]       phantom     'Phantom' tree data structure
 * \return                      0 on success,
 *                              1 on failure
 */
int capture_tree(const char *path, struct file **phantom)
{
    int reussite = capture_racine(path, (*phantom));
    return reussite;
}


int create_subDir(const char *path, struct file *phantom)
{
    if (path == NULL || phantom == NULL)
    {
        return 1;
    }

    char *rootpath = malloc(NAME_MAX * sizeof(char));
        if (rootpath == NULL) return 1;

    strcpy(rootpath, path);

    if (phantom->type == 0)
    {
        create_regular(rootpath, phantom);
        return 0;
    }
    if (phantom->type == 2)
    {
        create_link(rootpath, phantom);
        return 0;
    }
    if (phantom->type == 1)
    {
        struct file *next = phantom->attribute.child;
        create_directory(rootpath, phantom);
        char *newpath = malloc(NAME_MAX * sizeof(char));
        if (newpath == NULL) return 1;

        
        strcpy(newpath, rootpath);

        while (next != NULL)
        {
            switch (next->type)
            {
            case 0:
                create_regular(newpath, next);
                break;
            case 1:
                create_directory(newpath, next);
                strcat(newpath, "/%s");
                strcat(newpath, next->name);
                
                mkdir(newpath, next->mode);
                create_subDir(newpath, next);
                break;
            case 2:
                create_link(newpath, next);
                break;
            default:
                break;
            }
            next = next->next;
        }
        free(rootpath);
        free(newpath);
        free((struct file*)next);
        return 0;
    }
    free(rootpath);
    return 1;
}


int free_struct(struct file *to_free)
{
    if (to_free == NULL) return 1;
    
    if (to_free->type != 1)
    { 
        struct file *next = malloc(sizeof(struct file));
        if (next == NULL) {
            return 1;
        }
        while(to_free->next != NULL )
        {
            next = to_free->attribute.child->next;
            free((struct file *)to_free->attribute.child);
            to_free->attribute.child = next;
        }
        free((struct file*)to_free);
       
        return 0;
    } 
    else {
        free((struct file *)to_free);
        return 0;
    } 
    return 1;
}

/**
 * Create a 'phantom' tree following metadata previously stored
 *
 * \a Phantom data structure will be freed during the process.
 *
 * \param[in]       path        'Phantom' tree path
 * \param[in]       phantom     'Phantom' tree data structure
 * \return                      0 on success,
 *                              1 on failure
 */
int create_phantom(const char *path, struct file *phantom)
{
    if (path == NULL || phantom == NULL)
    {
        return 1;
    }

    char *rootpath = malloc(NAME_MAX * sizeof(char));
        if (rootpath == NULL) return 1;

    //sprintf(rootpath, "%s/%s", path, phantom->name);
    strcpy(rootpath, path);
    if (phantom->type == 0)
    {
        create_regular(rootpath, phantom);
        free(rootpath);

        return 0;
    }
    if (phantom->type == 2)
    {
        create_link(rootpath, phantom);
        free(rootpath);

        return 0;
    }
    if (phantom->type == 1)
    {
        struct file *next = phantom->attribute.child;
        create_directory(rootpath, phantom);
        char *newpath = malloc(NAME_MAX * sizeof(char));
        if (newpath == NULL) return 1;
        strcpy(newpath, rootpath);
        
        while (next != NULL)
        {
            switch (next->type)
            {
            case 0:
                create_regular(newpath, next);
                break;
            case 1:
                create_directory(newpath, next);
                strcat(newpath, "/");
                strcat(newpath, next->name);
                
                mkdir(newpath, next->mode);
                create_subDir(newpath, next);
                
                break;
            case 2:
                create_link(newpath, next);
                break;
            default:
                break;
            }
            next = next->next;
        }
        free(rootpath);
        free(newpath);
        return 0;
    }
    return 1;

    
}
