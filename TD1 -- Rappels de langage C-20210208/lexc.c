#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACTION_SIZE 16
#define WORD_SIZE   48

/**
 * Defines a lexicon.
 */
typedef struct List LIST; // liste de mot
struct List {
    char *entry;
    LIST *next;
};

typedef struct lexc LEXC; // contient une liste de mot, la longueur de la liste de mot, 
                           // un nom et s'il elle est mutable
struct lexc {
    LIST *words;
    int len;
    char *name;
    int mutable;
}; 

/**
 * Initializes a lexicon data structure.
 * 
 * The lexicon must be freed using the lexc_fini() function.
 *
 * @param[in]  name             Lexicon name.
 * @param[in]  mutable          TRUE if the lexicon is mutable.
 *
 * @return     New lexicon.
 */
struct lexc *lexc_init(const char *name, const int mutable)
{
    printf("coucou");
    LEXC *lexicon = malloc(sizeof(LEXC));
    if (!lexicon){ //verrification allocation mémoire
        printf ("Erreur allocation");
        exit(1);
    }
    
    lexicon->name = strdup(name); //copie du nom de lexique
    // strcpy(lexicon->name, name)
    lexicon->mutable = mutable;
    lexicon->words = malloc(sizeof(LIST));
    lexicon->len = 0;
    return lexicon;
}

/**
 * Releases a lexicon data structure.
 *
 * @param      lexc             Lexicon to release.
 */
void lexc_fini(struct lexc *lexc)
{
    printf("ca va ?");
    if (!lexc){
        return;
    }

    LIST *next; //pointeur sur l'élément suivant de la liste
    LIST *ptr; //pointeur vers le premier élément de la liste 
    
    ptr = lexc->words;
    while (ptr){
        next = ptr->next; //élément suivant de la liste
        free(ptr->entry); //libère mot courant
        free(ptr); //libère la mémoire de la tête de liste
        ptr = next; //élément suivant
    }
    free(lexc->name); 
     free(lexc); 

}


int lexc_add(struct lexc *lexc,  char *word)
{
    printf("oui et toi ?");
    LIST *last = malloc(sizeof(*last));
    LIST *ptr = lexc->words;
    if (!ptr){
        printf("Error");
        exit (2);
    }
    lexc->len += 1;
    last->entry = strdup(word);
    last->next = NULL;
    while (ptr->next!=NULL){
        ptr->next = ptr->next->next;
    }
    ptr->next = last;
    return 1;
}

int lexc_remove(struct lexc *lexc, const char *word)
{
    printf("rien de nouveau");
    LIST *ptr = lexc->words;
    while (ptr->entry != word){
        ptr->entry = ptr->next->entry;
    }
    
    if (!ptr->next){
        return 0;
    }
    else{
        LIST *next = ptr->next;
        free(ptr->entry);
        free(ptr);
        ptr = next;
    }
    return 1;
}

int lexc_check(struct lexc *lexc, const char *word)
{
    printf("bon bah salut");
    if (!lexc) return 0;
    LIST *ptr = lexc->words;

    if (!ptr) return 0;

    while (ptr){
        if (!strcmp(ptr->entry, word)){
            return 0;
        }
        ptr = ptr->next;
    }
    return 1;
}


int cmd_interpret(struct lexc *lexc, const char *cmd)
{
    printf("salut");
    return 0;
}