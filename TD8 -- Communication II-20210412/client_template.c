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
        fprintf (stderr, "Error size value_name.\n");
        return 1;
    }
    char mess[BUFF_SIZE];
    sprintf(mess,"0 %s %f", value_name, value);
    char new_name [BUFF_SIZE];
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);
    int op = open(new_name, O_WRONLY);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        close(op);
        return 1;
    }

    errno = 0;
    if (write(op, mess, BUFF_SIZE)<= 0){
        fprintf(stderr,"Errno write client :%d\n", errno);
        close(op);
        return 1;
    }
    else {
        close(op);
        
        return 0;
    }
    
}

/* TODO: send remove commant to the server */
int
dict_remove(const char *dict_name, const char *value_name)
{
    if(sizeof(value_name) > 64){
        fprintf (stderr,"Error size value_name.\n");
        return 1;
    }

    char new_name [BUFF_SIZE];
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);
    
    int op = open(new_name, O_WRONLY); 
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        close(op);
        return 1;
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        fprintf(stderr,"Malloc error.\n");
        free(mess);
        close(op);
        return 1;
    }
    sprintf(mess, "1 %s", value_name);
    if (write(op, mess, sizeof(mess))<=0){
        fprintf(stderr, "Error writting client pipe %s\n", new_name);
        free(mess);
        close(op);
        return 1;
    } 
    else {
        close(op);
        free(mess);
        return 0;
    }
    free(mess);
    
}

/* TODO: send ask command to the server */
int
dict_ask(const char *dict_name, const char *value_name)
{
    if(sizeof(value_name) > 64){
        fprintf (stderr, "Error size value_name.\n");
        return 1; 
    }
    char new_name [BUFF_SIZE];
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);
    
    int op = open(new_name, O_WRONLY);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);
        close(op);
        return 1;
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        fprintf(stderr,"Malloc error\n.");
        close(op);
        free(mess);
        
        return 1;
    }
    sprintf(mess, "2 %s", value_name);
    errno = 0;
   if (write(op, mess, sizeof(mess))<=0){
        fprintf(stderr,"errno write client :%d\n", errno);
        close(op);
        free(mess);
        
        return 1;
    }
    close(op);
    free(mess);
       
    char read_dict [BUFF_SIZE];
    sprintf(read_dict, "/tmp/dict_%s_out", dict_name);
    char * lect = malloc(264* sizeof(char));
   
    int rd = open(read_dict, O_RDONLY);
    if (rd < 0){
        fprintf(stderr, "Error opening client read end of pipe %s\n", read_dict);
        close(op);
        return 1;
    }
    errno = 0;
    if (read(rd, lect, sizeof(lect))<=0){
        fprintf(stderr,"Errno read client :%d\n", errno);
        close(op);
        free(lect);
        return 1;
    }else {
        if (strcmp(lect, "No value")!=0){
            printf("The value searched is %s\n", lect);
        }else {
            printf("The value is not in %s", dict_name);
        }
        free(lect);
        return 0;
    }
}

/* TODO: send exit command to the server */
int
dict_exit(const char *dict_name)
{
    char new_name [BUFF_SIZE];
    sprintf(new_name, "/tmp/dict_%s_in", dict_name);

    int op = open(new_name, O_WRONLY);
    if (op < 0){
        fprintf(stderr, "Error opening client write end of pipe %s\n", new_name);

        return 1;
    }

    char * mess = malloc(264* sizeof(char));
    if (mess == NULL)  
    {
        fprintf(stderr,"Malloc error.\n");
        close(op);
        free(mess);
        return 1;
    }
    sprintf(mess, "3 %s", dict_name);
    errno = 0;
    if (write(op, mess, sizeof(mess))<=0){
        fprintf(stderr,"Errno write client :%d\n", errno);
        close(op);
        free(mess);
        return 1;
        
    } 
    else {
        close(op);
        free(mess);
        
        return 0;
    }
    
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
