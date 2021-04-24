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
#include <errno.h>

#define DICT_NAME_MAX 64
#define BUFF_SIZE 264

/* entry of a dictionary, consists in a name and its value */
struct dictionary_item
{
    char name[DICT_NAME_MAX];
    double value;
    //struct dictionary_item *suiv;
    //int max_size;
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

/* allocate the dictionary structure */
int create_dict(const char *size_string, struct dictionary_item **dict, size_t *dict_max_size)
{
    *dict_max_size = strlen(size_string);
    *dict = malloc((*dict_max_size) * sizeof(struct dictionary_item *));
    if (dict != NULL)
    {
        printf("Dictionary created.\n");
        return 0;
    }
    printf("PBM Malloc.\n");
    return 1;
}

void affiche_dict(struct dictionary_item *dict, size_t dict_max_size){
    for (int i = 0; i < dict_max_size; i ++){
        printf("i : %d --> name : %s --> value : %f\n", i, dict[i].name, dict[i].value);
    }
}

/* free the dictionary structure */
void destroy_dict(struct dictionary_item *dict)
{
    free(dict);
    printf("Dictionary destroyed.\n");
}

/* destroy the pipe files used by the server */
void destroy_pipes(const char *dict_name)
{
    char in[75];
    char out[75];
    sprintf(in,"/tmp/dict_%s_in",dict_name);
    sprintf(out,"/tmp/dict_%s_out",dict_name);
    
    unlink(in);
    unlink(out);
    printf("Pipes destroyed.\n");
}

/* create the pipe files used by the server */
int create_pipes(const char *dict_name)
{
    if (sizeof(dict_name) > DICT_NAME_MAX)
    {
        printf("Name too long.\n");
        return 1;
    }

    char in[75];
    char out[75];
    sprintf(in,"/tmp/dict_%s_in",dict_name);
    sprintf(out,"/tmp/dict_%s_out",dict_name);

    char **name_serv_pipe = malloc(2*sizeof(char *));
    name_serv_pipe[0] = in;
    name_serv_pipe[1] = out;

    for (int i = 0; i < 2; i ++){
        if (mkfifo(name_serv_pipe[i], 0644) < 0 && errno == 17)
        {
            unlink(name_serv_pipe[i]);
            if (mkfifo(name_serv_pipe[i], 0644)<0){
                fprintf(stderr, "error opening write end of pipe %s\n", name_serv_pipe[i]);
                exit(1);
            }
        }
        printf("Pipe : %s created.\n", name_serv_pipe[i]);
        close(*name_serv_pipe[i]);
    }

    return 0;
}

/* open a pipe for the given dictionary, input or output side */
int open_pipe(const char *dict_name, bool dict_input)
{
    char *name_serv_pipe = malloc(sizeof(dict_name));
    sprintf(name_serv_pipe, "/tmp/dict_%s_%s", dict_name, dict_input ? "in" : "out");
    printf("Try opening %s...\n", name_serv_pipe);
    int fd;
    fd = open(name_serv_pipe, dict_input ? (O_RDONLY) : (O_WRONLY|O_NONBLOCK));             
    if (fd < 0)
    {
        //printf("errno : %d\n", errno);
        fprintf(stderr, "Error opening server read end of pipe : %s\n", dict_name);
        return 1;
    }
    printf("Opened in server correctly %s.\n", name_serv_pipe);
    return 0;
}

/* close a pipe */
void close_pipe(int fd)
{
    if (close(fd)){
        printf("Closed in server correctly.\n");
    }
}

/* receive a command from the input pipe */
int receive_command(int fd, struct command *cmd, const char *dict_name)
{
    close_pipe(0);
    char *name_serv_pipe = malloc(sizeof(dict_name));
    sprintf(name_serv_pipe, "/tmp/dict_%s_in", dict_name);

    char full_cmd [BUFF_SIZE];
    //= malloc(BUFF_SIZE*sizeof(char));
    int op = open(name_serv_pipe, O_RDONLY);
    if (op == 0){
        if(read(op, full_cmd, sizeof(BUFF_SIZE))){
            printf("Reading command...\n");
           
            char orders[4];
            printf("full cmd : %s\n", full_cmd);
            char *strTok = strtok(full_cmd, " \0");
            int i = 0;
            while(strTok != NULL){
                    printf("'%s'\n", strTok);
                    orders[i] = *strTok;
                    printf("order %d : %s\n", i, &orders[i]);
                    i ++;
                    strTok = strtok(NULL, " \0");
                    
            }
            cmd->op = atoi(orders[0]);
            if (cmd->op = 4){
                printf("cmd.op = %d\n", cmd->op);
                printf("polaire servers\n");
                destroy_pipes(dict_name);
                exit (0);
            }
            printf("commande : %d\n", cmd->op);
            strcpy(cmd->name, &orders[1]);
            printf("cmd->name: %s\n",cmd->name);

            if (orders[2]){
                cmd->value = orders[2];
                printf("cmd->value: %f\n",cmd->value);
            }
            printf("Command received.\n");
            free(name_serv_pipe);
            free(orders);
            free(strTok);
        }
        else{
            printf ("read errno: %d\n", errno);
            return 1;
        }   
    }
    else {
        printf ("open errno: %d\n", errno);
        printf("Error opening server read side of %s.\n",name_serv_pipe);
        return 1;
    }
    close_pipe(fd);
    return 0;

}

/* add an entry to the dictionary */
int dict_add(struct dictionary_item *dict, size_t *dict_count, size_t dict_max_size, struct command cmd)
{   
    for (int i = 0; i < dict_max_size; i ++ ){
        if (&dict[i] == 0){
            strcpy(dict[i].name, cmd.name);
            dict[i].value = cmd.value;
            dict_count = dict_count + 1;
            printf("Added to Dictionary %s : %s\n", dict->name, cmd.name);
            affiche_dict(dict, dict_max_size);
            return 0;
        }
    }
    return 1;
}

/* remove an entry from the dictionary */
int dict_remove(struct dictionary_item *dict, size_t *dict_count, struct command cmd)   
// a revoir mouvement de tableau
{
    int i = 0;
    while (&dict[i] != 0){
        if (cmd.name == dict[i].name){

            for (int j = i+1; j < *dict_count; j++) {
                if (dict[j].value == 0){
                    dict[i] = dict[j];
                    dict[j].value = 0;
                    strcpy(dict[j].name, " ");
                    dict_count = dict_count - 1;
                }
            }            
        }
    }
    affiche_dict(dict, *dict_count);

    return 1;
}

/* ask the value of a dictionary entry, if exists */
int dict_ask(struct dictionary_item *dict, size_t dict_count, struct command *cmd)
{
    int i = 0;
    while (&dict[i] != 0){
        if (cmd->name == dict[i].name){
            cmd->value = dict[i].value;
           return 0; 
        }
    }
    return 1;
}

/* send the response of an ask request */
int send_response(int fd, int error, double value, const char *dict_name)
{
    close_pipe(fd);
    char *name_serv_pipe = malloc(sizeof(dict_name));
    sprintf(name_serv_pipe, "/tmp/dict_%s_out", dict_name);
    int op = open(name_serv_pipe, O_WRONLY);
    if (value){
        write(op, "The search value is : %d", value);
    }
    else if (error){
        write(op, "Is not in %s.", *dict_name);
    }
    close (op);

    free(name_serv_pipe);
    return 1;
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

    rc = create_pipes(argv[1]);
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
        rc = receive_command(fd[0], &cmd, argv[1]);
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

            rc = send_response(fd[1], rc2, cmd.value, argv[1]);
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
