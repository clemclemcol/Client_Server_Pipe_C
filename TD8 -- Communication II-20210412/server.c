#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


#define DICT_NAME_MAX 64
#define BUF_SIZE 16

/* entry of a dictionary, consists in a name and its value */
struct dictionary_item
{
    char name[DICT_NAME_MAX];
    double value;
    struct dictionary_item * suiv;
};

/* operators of a dictionary */
enum command_op
{
    OP_ADD,    /* add an entry */
    OP_REMOVE, /* remove an entry */
    OP_ASK,    /* ask about an entry */
    OP_EXIT    /* terminate server */
};

/* command extracted from the input pipe */
struct command
{
    enum command_op op;
    char name[DICT_NAME_MAX];
    double value;
};

struct dictionary_item create_item(char * name, double value , struct dictionary_item * suiv){
    struct dictionary_item item;
    if (sizeof(name) <= DICT_NAME_MAX){
        strcpy(item.name, name);
    }
    else {
        printf("Dict name too long.\n");
        return;
    }
    
    item.value = value;
    item.suiv = NULL;

    return item;
}

/* allocate the dictionary structure */
int create_dict(const char *size_string, struct dictionary_item **dict, size_t *dict_max_size)
{
    *dict_max_size = strlen(size_string);

    dict = malloc(sizeof(struct dictionary_item*));
    if (!dict) return 1;

    return 0;
}

/* free the dictionary structure */
void destroy_dict(struct dictionary_item *dict)
{
    while (dict->suiv != NULL){
        dict->suiv = dict->suiv->suiv;
    }
    free(dict);
}

/* destroy the pipe files used by the server */
void destroy_pipes(const char *dict_name)
{
    unlink(dict_name);
    printf("Pipe destroyed.\n");
}

/* create the pipe files used by the server */
int create_pipes(const char *dict_name, bool dict_input)
{
    mkdir("/tmp", 0777 );
    char * name_serv_pipe = malloc(sizeof(dict_name));
    sprintf(name_serv_pipe, "/tmp/dict_%s_%s", dict_name, dict_input ? "in" : "out");

    printf("creation : %s\n", name_serv_pipe);  // RESTE BLOQUER LA 
    int fd = open(name_serv_pipe, O_WRONLY );
    printf("input : %d\n", dict_input);
    //close(fd);
    if (fd){
       
        printf("Pipe already exists.\n");
        destroy_pipes(name_serv_pipe);
    }
    //close(fd);
    if (mkfifo(name_serv_pipe, 0666) < 0){
        
        fprintf(stderr, "Error creating pipe at %s\n", name_serv_pipe);
        return 1;
    } 
                                                              //1 --> lecture; 0 --> écriture
    printf("Pipe %s created.\n", name_serv_pipe);
    close (name_serv_pipe);
    return 0;
}


/* open a pipe for the given dictionary, inp ut or output side */
int open_pipe(const char *dict_name, bool dict_input)
{
    char * name_serv_pipe = malloc(sizeof(dict_name));
    sprintf(name_serv_pipe, "/tmp/dict_%s_%s", dict_name, dict_input ? "in" : "out");
    printf("open client : %s\n", name_serv_pipe);
    int fd;
    if (dict_input == 0){
        fd = open(name_serv_pipe, O_WRONLY );
    }
    else if (dict_input == 1){
        fd = open(name_serv_pipe, O_RDWR );
    }
    printf ("fd : %d\n", fd);

    if (fd < 0){
        fprintf(stderr, "Error opening read end of pipe : %s\n", dict_name);
        return 1;
    }        
    printf("Opened correctly %s.\n", name_serv_pipe);
    close(name_serv_pipe);
    return 0;
}

/* close a pipe */
void close_pipe(int fd)
{
    close(fd);
    printf("Closed correctly.\n");
}

/* receive a command from the input pipe */
int receive_command(int fd, struct command *cmd)
{
    char size_cmd[BUF_SIZE];
    read(fd, size_cmd, sizeof(BUF_SIZE));
    read(fd, cmd, sizeof(struct command));
    close(fd);
    printf("Received %s.\n", cmd->name);
    return 0;
}

/* ask the value of a dictionary entry, if exists */
int dict_ask(struct dictionary_item *dict, size_t dict_count, struct command *cmd)
{
    for (int i = 0; i < dict_count; i++){
        if (strcmp(dict[i].name, cmd->name) == 0){
            printf("Is in dictionary: %d.\n", i);
            return i;
        }
    }
    printf("Is not in dictionary.\n");
    return -1;
}

/* add an entry to the dictionary */
int dict_add(struct dictionary_item *dict, size_t *dict_count, size_t dict_max_size, struct command cmd)
{
    int res = dict_ask(dict, dict_count, &cmd);
    if (res != -1){
        for (int i =0; i< dict_max_size; i++){
            if (dict[i].value == 0){
                strcpy(dict[i].name, cmd.name);
                dict[i].value = cmd.value;
                printf("Well added.\n");
                return 0;
            }
        }
    }
    printf("Already in dictionary.\n");
    return 1;
}

/* remove an entry from the dictionary */
int dict_remove(struct dictionary_item *dict, size_t *dict_count, struct command cmd)
{
    int res = dict_ask(dict, dict_count, &cmd);
    if (res != -1){
        dict[res].value = 0;
         strcpy(dict[res].name, NULL);
        printf("Well removed.\n");
        return 0;
    }
    printf("Can not remove unexistant item.\n");
    return 1;
}

/* send the response of an ask request */
int send_response(int fd, int error, double value)
{
    if (error){
        write(fd, &error, sizeof(int));
        printf("wrote %d\n", error);
        close(fd);
        return 1;
    } 
    else {
        write(fd, &value, sizeof(double));
        printf("wrote %f\n", value);
        close(fd);
        
    }
    return 0;
} 


/* main function, contains the server loop */
int main(int argc, char **argv)
{
    struct dictionary_item *dict;
    size_t dict_count = 0;
    size_t dict_max_size;
    bool done = false;
    int fd[2] = {-1, -1};
    int rc;
    char c;

    if (argc != 3)
    {
        fprintf(stderr,
                "[ERR] usage: %s name size\n"
                "             name -- dictionary name\n"
                "             size -- maximum dictionary size\n",
                argv[0]);
        rc = 1;
        goto err_exit;
    }

    rc = create_dict(argv[2], &dict, &dict_max_size);
    if (rc)
        goto err_exit;

    rc = create_pipes(argv[1], true);
     
    if (rc)
        goto err_dict;

    do
    {
        struct command cmd;
        int rc2;
        
        fd[0] = open_pipe(argv[1], true);
        if (fd[0] < 0)
        {
            rc = 1;
            goto err_pipe;
        }

        rc = receive_command(fd[0], &cmd);
        if (rc)
            break;

        switch (cmd.op)
        {
        case OP_ADD:
            rc2 = dict_add(dict, &dict_count, dict_max_size, cmd);
            break;
        case OP_REMOVE:
            rc2 = dict_remove(dict, &dict_count, cmd);
            break;
        case OP_ASK:
            rc2 = dict_ask(dict, dict_count, &cmd);
            fd[1] = open_pipe(argv[1], false);
            if (fd[1] < 0)
            {
                rc = 1;
                goto err_pipe;
            }

            rc = send_response(fd[1], rc2, cmd.value);
            close_pipe(fd[1]);
            break;
        case OP_EXIT:
            fprintf(stderr, "[INF] will quit\n");
            done = true;
        };

        if (rc)
            break;

        close_pipe(fd[0]);
    } while (!done);

err_pipe:
    close_pipe(fd[0]);
    destroy_pipes(argv[1]);

err_dict:
    destroy_dict(dict);

err_exit:
    return rc;
}
