#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DICT_NAME_MAX 64

struct dictionary_item
{
    char name[DICT_NAME_MAX];
    double value;
};

int create_dict(const char *size_string, struct dictionary_item **dict, size_t *dict_max_size)
{
    *dict_max_size = strlen(size_string);
    dict = malloc(*dict_max_size * sizeof(struct dictionary_item*));
    if (!dict) return 1;
    return 0;
}

int dict_add(struct dictionary_item *dict, size_t *dict_count, size_t dict_max_size, struct command cmd)
{
    create_dict(cmd->name, dict, dict_max_size);
    return 0;
}

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
    

/*err_pipe:
    close_pipe(fd[0]);
    destroy_pipes(argv[1]);

err_dict:
    destroy_dict(dict);*/

err_exit:
    return rc;
}