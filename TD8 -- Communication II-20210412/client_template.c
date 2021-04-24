#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>

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

/* open a pipe for the given dictionary, input or output side */
int
open_pipe(const char *dict_name, bool dict_input)
{
    char buf[256];
    int fd;
    //sprintf(buf, "/tmp/dict_%s_%s", dict_name, dict_input ? "in" : "out");
    printf("Pipe to be opened : %s\n", dict_name);
    return open(buf, dict_input ? O_WRONLY/O_NONBLOCK : O_RDONLY);
}

/* TODO: send add command to the server */
int
dict_add(const char *dict_name, const char *value_name, double value)
{ 
    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        return 1;
    }

    char * mess = malloc(64* sizeof(char));
    char *str_val = malloc(sizeof(char));
    if (mess == NULL || str_val == NULL)  
    {
        printf("Malloc error.\n");
        return 1;
    }
    sprintf(str_val, "%f", value);
    strcpy(mess, "0");
    strcat(mess, " ");
    strcat(mess, value_name);
    strcat(mess, " ");
    strcat(mess, str_val);
    strcat(mess, "\0");
    //mess = "123456789 abcdefghij";
    printf("Message to write : %s .\n", mess);
    printf("size of mess: %ld\n", sizeof(mess));

    char *new_name = malloc(sizeof(char*));
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);
    //printf("dict_name : %s\n", new_name);
    int op = open(new_name, O_WRONLY);
    //printf("%d --> %d \n ", op, errno);
    //printf("errno : %d\n", errno);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        return 1;
    }
    printf("Opened in client correctly %s.\n", new_name);

    printf("Adding...\n");
    if (write(op, mess, sizeof(mess)) != -1){
        
        //printf("errno write client :%d\n", errno);
        printf("Message wrote by client in %s successfuly.\n", new_name);
    }
    
    close(op);

    free(mess);
    free(str_val);
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
    
    int op = open_pipe(new_name, true); 
    printf("op : %d\n", op);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        return 1;
    }

    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        return 1;
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        printf("Malloc error.\n");
        return 1;
    }
    
    strcpy(mess, "1 ");
    strcat(mess, dict_name);
    strcat(mess, " ");
    strcat(mess, value_name);
    strcat(mess, " |");

    printf("Message to write : %s .\n", mess);

    if (write(op, mess, sizeof(mess))){
        //printf("errno write client :%d\n", errno);
        printf("Message wrote by client in %s successfuly.\n", new_name);
    }
    printf("coucou");
    if (close(op)){
        printf("Closed in client correctly.\n");
    }else {
        printf("errno close client :%d\n", errno);
    }
    
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
    
    //int op = open_pipe(new_name, true);
    int op = open(new_name, O_WRONLY);
    //printf("op : %d\n", op);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        return 1;
    }

    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        return 1;
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        printf("Malloc error\n.");
        return 1;
    }
    
    strcpy(mess, "2 ");
    strcat(mess, dict_name);
    strcat(mess, " ");
    strcat(mess, value_name);
    strcat(mess, " |");

    printf("Message to write : %s .\n", mess);

   if (write(op, mess, sizeof(mess))){
        //printf("errno write client :%d\n", errno);
        printf("Message wrote by client in %s successfuly.\n", new_name);
    }
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
        return 1;
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        printf("Malloc error.\n");
        return 1;
    }
    
    strcpy(mess, "3 ");
    strcat(mess, dict_name);
    strcat(mess, " |");
    printf("Message to write : %s .\n", mess);

    if (write(op, mess, sizeof(mess))){
        //printf("errno write client :%d\n", errno);
        printf("Message wrote by client in %s successfuly.\n", new_name);
    }
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
