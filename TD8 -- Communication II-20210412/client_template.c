#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>

#define BUFF_SIZE 256

/* print usage message and return nonnull rc */
int
usage(const char *prog)
{
    fprintf(stderr,
            "[ERR] usage: %s dict action [opt1 opt2]\n"
            "             action: + name value\n"
            "                     - name\n"
            "                     ? name\n"
            "                     !\n",
            prog);

    return 1;
}

/* print error message on stderr and return nonnull rc */
int
log_error(const char *msg)
{
    fprintf(stderr, "[ERR] %s\n", msg);

    return 1;
}


/* TODO: send add command to the server */
int
dict_add(const char *dict_name, const char *value_name, double value)
{ 
    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        return 1;
    }

    char mess[BUFF_SIZE];

    sprintf(mess,"0 %s %f", value_name, value);
    printf("Message to write : %s .\n", mess);

    char *new_name = malloc(sizeof(char*));
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);
    int op = open(new_name, O_WRONLY);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        close(op);
        free(new_name);
        return 1;
    }
    printf("Opened in client correctly %s.\n", new_name);

    printf("Adding...\n");
    errno = 0;
    if (write(op, mess, BUFF_SIZE)<0){
        printf("errno write client :%d\n", errno);
        close(op);
        free(new_name);
        return 1;
        
    }
    printf("Message wrote by client in %s successfuly.\n", new_name);
    close(op);
    free(new_name);
    return 0;
}

/* TODO: send remove commant to the server */
int
dict_remove(const char *dict_name, const char *value_name)
{
    printf("Removing...\n");
    char *new_name = malloc(sizeof(char*));
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);
    
    int op = open(new_name, O_WRONLY); 
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        free(new_name);
        close(op);
        return 1;
    }

    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        free(new_name);
        close(op);
        return 1;
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        printf("Malloc error.\n");
        free(mess);
        free(new_name);
        close(op);
        return 1;
    }
    sprintf(mess, "1 %s", value_name);
    printf("Message to write : %s .\n", mess);

    if (write(op, mess, sizeof(mess))<0){
        //printf("errno write client :%d\n", errno);
        free(mess);
        free(new_name);
        close(op);
       return 1;
    } 
    errno = 0;
    printf("Message wrote by client in %s successfuly.\n", new_name);
    close(op);

    free(mess);
    free(new_name);

    return 0;
}

/* TODO: send ask command to the server */
int
dict_ask(const char *dict_name, const char *value_name)
{
    printf("Asking...\n");
    char *new_name = malloc(sizeof(char*));
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);
    
    int op = open(new_name, O_WRONLY);
    //printf("op : %d\n", op);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        close(op);
        free (new_name);
        return 1;
        
    }

    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        close(op);
        free (new_name);
        return 1;
        
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        printf("Malloc error\n.");
        close(op);
        free(mess);
        free (new_name);
        return 1;
        
    }
    sprintf(mess, "2 %s", value_name);
    printf("Message to write : %s .\n", mess);
    errno = 0;
   if (write(op, mess, sizeof(mess))<0){
        printf("errno write client :%d\n", errno);
        close(op);
        free(mess);
        free (new_name);
        return 1;
    }
    printf("Message wrote by client in %s successfuly.\n", new_name);
    close(op);

    free(mess);
    free (new_name);
    return 0;
}

/* TODO: send exit command to the server */
int
dict_exit(const char *dict_name)
{
    printf("Quitting...\n");

    char *new_name = malloc(sizeof(char*));
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);

    int op = open(new_name, O_WRONLY);

    printf("op : %d\n", op);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        free(new_name);
        return 1;
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        printf("Malloc error.\n");
        close(op);
        free(mess);
        free(new_name);
        return 1;
    }
    sprintf(mess, "3 %s", dict_name);
    printf("Message to write : %s .\n", mess);
    errno = 0;
    if (write(op, mess, sizeof(mess))<0){
        printf("errno write client :%d\n", errno);
        close(op);
        free(mess);
        free(new_name);
        return 1;
        
    }
    printf("Message wrote by client in %s successfuly.\n", new_name);
    close(op);

    free(mess);
    free(new_name);
    return 0;
}

/* client main function, literally parse the program args
 * and then send the command to the desired server */
int main(int argc, char **argv)
{
    if (argc < 3)
        return usage(argv[0]);

    if (strcmp(argv[2], "+") == 0) {
        if (argc != 5)
            return usage(argv[0]);

        return dict_add(argv[1], argv[3], atof(argv[4]));
    } else if (strcmp(argv[2], "-") == 0) {
        if (argc != 4)
            return usage(argv[0]);

        return dict_remove(argv[1], argv[3]);
    } else if (strcmp(argv[2], "?") == 0) {
        if (argc != 4)
            return usage(argv[0]);

        return dict_ask(argv[1], argv[3]);
    } else if (strcmp(argv[2], "!") == 0) {
        if (argc != 3)
            return usage(argv[0]);

        return dict_exit(argv[1]);
    }

    return usage(argv[0]);
}
