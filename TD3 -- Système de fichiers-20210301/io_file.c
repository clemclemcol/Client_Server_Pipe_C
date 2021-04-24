#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "io_file.h"

// Ouverture du fichier
// \param	path			Nom/chemin du fichier à ouvrir
// \param	access			Accès au fichier (cf. flags de open() )
// \return					Structure IO_FILE (le descripteur est mis à -1
//							si l'ouverture a échouée)
IO_FILE IO_open(const char *path, int access){
    IO_FILE ret;
    ret.path = strdup(path);
    ret.access = access;

    if (!path){
        ret.desc = -1;
        return ret;
    } 

    if (access & O_CREAT){
        ret.desc = open(path, O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR);
        return ret;
    }
    else {
        ret.desc = open(path, access);
    }

    return ret;
}

// Fermeture du fichier
// \param	file			Structure IO_FILE à fermer
// \return					-1 si échec, 0 sinon
int IO_close(IO_FILE file){
    if(file.desc < 0) return -1;
    
    int r = close (file.desc);
    free(file.path);
    return r;

}

// Suppresion de fichier
// \param	path			Nom/chemin du fichier à supprimer
// \return					-1 si échec, 0 sinon
int IO_remove(const char *path){
    IO_FILE ret;
    ret.path = strdup(path);

    ret.desc = remove(ret.path);
    if (ret.desc == 0) return 0;
    else return -1;

}

// Lecture d'un caractère à partir d'un fichier
// \param	file			Structure IO_FILE où lire
// \param	c				Pointeur du caractère lu
// \return					-1 si échec, 0 si fin du fichier,
//							nombres d'octets lus sinon
int IO_char_read(IO_FILE file, char *c){
    if (file.desc < 0) {
        printf("Mauvais descripteur.");
        return -1;
    }
    if (!c) {
        printf("file null.");
        return -1;
    }
    if (!((file.access == O_RDONLY) || (file.access & O_RDWR))) {
        printf("L'acces n'est pas autorisé.");
        return -1;
    } 
    int r = read(file.desc, c, 1);
    if (r<0){
        printf("erreur lecture");
        return -1;
    }
    return r;
}

// Ecriture d'un caractère dans un fichier
// \param	file			Structure IO_FILE où écrire
// \param	c				Caractère à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int IO_char_write(IO_FILE file, const char c){
    if (file.desc < 0) return -1;
    if (!c) {
        printf("File null.");
        return -1;
    }
    if (!((file.access == O_WRONLY) || (file.access & O_RDWR))) {
        printf("L'acces n'est pas autorisé.");
        return -1;
    }
    int w = write (file.desc, &c, 1);
    if (w < 0) 

    return w;
}

/*
// Lecture d'une chaîne de caractère dans un fichier
// \param	file			Structure IO_FILE où lire
// \param	string			Chaîne lue
// \param	maxSize			Taille max de la chaîne lue
// \return					-1 si échec, 0 si fin du fichier,
//							nombre d'octets lus sinon
int IO_string_read(
	IO_FILE file,
	char *string,
	int maxSize);

// Ecriture d'une chaîne de caractère dans un fichier
// \param	file			Structure IO_FILE où écrire
// \param	string			Chaîne de caractère à écrire
// \param	size			Taille de la chaîne à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int IO_string_write(
	IO_FILE file,
	const char *string,
	int size);

// Lecture d'un entier dans un fichier
// \param	file			Structure IO_FILE où lire
// \param	n				Entier lu
// \return					-1 si échec, 0 si fin du fichier,
//							nombre d'octets lus sinon
int IO_int_read(
	IO_FILE file,
	int *n);

// Ecriture d'un entier dans un fichier
// \param	file			Structure IO_FILE où écrire
// \param	n				Entier à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int IO_int_write(
	IO_FILE file,
	const int n);
*/