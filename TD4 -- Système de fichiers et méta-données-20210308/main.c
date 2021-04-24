#include <stdio.h>
#include <stdlib.h>
#include "internals.h"
#include "api.h"


int main(int argc, char **argv)
{
    struct file *fichier = malloc(sizeof(struct file*));
    if (argc>2){
        capture_tree(argv[1],&fichier);    
        create_phantom(argv[2], fichier);    
    }
    else {
        printf("Erreur argument.\n");
        free_struct(fichier);

        return 1;
    }
    free_struct(fichier);
    

   
    return 0;
}