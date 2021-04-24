#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>

#define HOME "//mnt/c/Users/clemc/OneDrive/Bureau/UVSQ"

//exo1
void transfert_char(int fd[2], const char *str, pid_t p)
{
    char *buff = malloc(16* sizeof(char));
    if (p)
    {
        close(fd[0]);
        write(fd[1], str, 16 * sizeof(char));
        close(fd[1]);
        wait(NULL);
        printf("%d -> %s\n", getpid(), buff);
    }
    else
    {
        close(fd[1]);
        read(fd[0], buff, 16 * sizeof(char));
        close(fd[0]);
        printf("%d -> %s\n", getpid(), buff);
    }
}

void transfert_entiers(int fd[2], const int a, const int b, pid_t p)
{
    char *buff1 = malloc(sizeof(int));
    char *buff2 = malloc(sizeof(int));
    char* acopy = malloc(sizeof(int));
    char* bcopy = malloc(sizeof(int));
    if (p)
    {
        close(fd[0]);
        sprintf(acopy, "%d", a);
        sprintf(bcopy, "%d", b);
        write(fd[1], &a, sizeof(int));
        write(fd[1], &b, sizeof(int));
        close(fd[1]);
        wait(NULL);
        printf("%d -> %s\n", getpid(), buff1);
        printf("%d -> %s\n", getpid(), buff2);
    }
    else
    {
        close(fd[1]);
        read(fd[0], buff1,  sizeof(int));
        read(fd[0], buff2,  sizeof(int));
        close(fd[0]);
        printf("%d -> %s\n", getpid(), buff1);
        printf("%d -> %s\n", getpid(), buff2);
    }

}

//exo 2 

void affichage(){ //marche par 
    time t;
    t = time (NULL);
    struct tm *t_info = localtime(&t);
    char day[15];
    char **message = malloc(sizeof(char*));
    message[0] = "Bonjour, ceci est un affichage informatisé.";
    sprintf(message[1], "%d/%d/%d - %d:%d:%d\n", t_info->tm_wday,t_info->tm_mon, t_info->tm_year,
                                                    t_info->tm_hour, t_info->tm_min, t_info->tm_sec);

}

void main()
{
    const char *str = "Hello world!";
    const int a = 23;
    const int b = 2;
    int fd_pipe[2];
    
    pid_t p;

    if (pipe(fd_pipe) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    p = fork();
    if (p == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    /*transfert_char(fd_pipe, str, p);
    transfert_entiers(fd_pipe, a, b, p);*/

    affichage();


    exit(EXIT_SUCCESS);
}