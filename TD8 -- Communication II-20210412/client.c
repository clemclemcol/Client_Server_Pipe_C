#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* print usage message and return nonnull rc */
int usage(const char *prog)
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
int log_error(const char *msg)
{
    fprintf(stderr, "[ERR] %s\n", msg);

    return 1;
}

/* open a pipe for the given dictionary, input or output side */
int open_pipe(const char *dict_name, bool dict_input)
{
    char buf[256];
    int fd;

    sprintf(buf, "/tmp/dict_%s_%s", dict_name, dict_input ? "in" :  "out");
    //printf("%d\n", dict_input);
    //printf("%s\n", buf);
    if (dict_input == 1){
        return open(buf, O_WRONLY);
    }
    else if (dict_input == 0){
        return open(buf, O_RDWR);
    }
    
    return -2;
}

/* TODO: send add command to the server */
int dict_add(const char *dict_name, const char *value_name, double value)
{
    printf("ajout ...\n");
    int op = open_pipe(dict_name, true); // PBM : op = -1 
    printf ("op : %d\n", op);

    if (op < 0){
        fprintf(stderr, "Error opening write end of pipe : %s\n", dict_name);
        return 1;
    }                    
    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        return 1;
    }
    char * com, * val ;
    strcpy(com, "OP_ADD ");
    strcat(com, value_name);
    strcat(com, " ");
    sprintf(val,"%f",value);
    strcat(com, val );
    write(op, com, sizeof(com));

    close(op);
    free(com);
    free(val);
    return 0;
}

/* TODO: send remove command to the server */
int dict_remove(const char *dict_name, const char *value_name)
{
    int op = open_pipe(dict_name, false);
    if (op < 0){
        fprintf(stderr, "Error opening write end of pipe %s\n", dict_name);
        return 1;
    }
    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        return 1;
    }
    char *com ;
    strcpy(com, "OP_REMOVE ");
    strcat(com, value_name);
    write (op, sizeof(com), sizeof(sizeof(com)));
    write(op, com, sizeof(com));
    close(op);
    return 0;
}

/* TODO: send ask command to the server */
int dict_ask(const char *dict_name, const char *value_name)
{
    int op = open_pipe(dict_name, false);
    if (op < 0){
        fprintf(stderr, "Error opening write end of pipe %s\n", dict_name);
        return 1;
    }
    if(sizeof(value_name) > 64){
        printf ("Error size value_name.\n");
        return 1;
    }
    char *com ;
    strcpy(com, "OP_ASK ");
    strcat(com, value_name);
    write (op, sizeof(com), sizeof(sizeof(com)));
    write(op, com, sizeof(com));
    close(op);
    return 0;
}

/* TODO: send exit command to the server */
int dict_exit(const char *dict_name)
{
    int op = open_pipe(dict_name, false);
    if (op < 0){
        fprintf(stderr, "Error opening write end of pipe %s\n", dict_name);
        exit(1);
    }
    write (op, sizeof("OP_EXIT"), sizeof(sizeof("OP_EXIT")));
    write(op, "OP_EXIT", sizeof("OP_EXIT"));
    close(op);
    return 1;
}

/* client main function, literally parse the program args
 * and then send the command to the desired server */
int main(int argc, char **argv)
{
    if (argc < 3)
        return usage(argv[0]);

    if (strcmp(argv[2], "+") == 0)
    {
        if (argc != 5)
            return usage(argv[0]);

        return dict_add(argv[1], argv[3], atof(argv[4]));
    }
    else if (strcmp(argv[2], "-") == 0)
    {
        if (argc != 4)
            return usage(argv[0]);

        return dict_remove(argv[1], argv[3]);
    }
    else if (strcmp(argv[2], "?") == 0)
    {
        if (argc != 4)
            return usage(argv[0]);

        return dict_ask(argv[1], argv[3]);
    }
    else if (strcmp(argv[2], "!") == 0)
    {
        if (argc != 3)
            return usage(argv[0]);

        return dict_exit(argv[1]);
    }

    return usage(argv[0]);
}
