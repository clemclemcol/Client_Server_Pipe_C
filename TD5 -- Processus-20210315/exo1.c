#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void fork_test(){
    pid_t p;
    p = fork();
    if (p){
        fprintf(stdout, "Hello World père\n");
        wait(NULL);
    }else {
        sleep(1);
        fprintf(stdout, "Hello world fils\n");
    }
}

void fork_pid_test(){
    pid_t p;
    p = fork();
    if (p){
        fprintf(stdout, "PID père :%d __ PID file : %d\n", getpid(), p);
        wait(NULL);
    }else {
        sleep(15);
        fprintf(stdout, "PID fils :%d __ PID père : %d\n", getpid(), getppid());
    }
}

void fork_rand_test(){
    pid_t p;
    int rand_fils, rand_pere;
    srand(time(NULL));
    p = fork();
    if(p){
        wait(&rand_pere);
        fprintf(stdout, "père : %d\n", WEXITSTATUS(rand_pere));

    }else{
        rand_fils = 1 + (rand() % 50);
        fprintf(stdout, "fils : %d\n", rand_fils);
        exit(rand_fils);
    }
}

//5.2
void child_sleep_and_pid(int t){
    sleep(t);
    fprintf(stdout, "%d [%d]\n", getpid(), t);
    exit(t);
}

void fork_sleep_and_wait(){
    pid_t p;
    int i;
    int t;
    srand(time(NULL));
    for(i=0;i<10;i++){
        p = fork();
        t = rand() % 10 + 1;
        if(!p) child_sleep_and_pid(t);
    }
    for(i=0; i<10; i++){
        p = wait(&t);
        fprintf(stdout, "%d fini en %d secondes\n", p, WEXITSTATUS(t));
    }
}

void child_1(){
    fprintf(stdout, "child %d", getpid());
    exit(0);
}

void multiple_fork(int n, int m){
    pid_t p;
    int i, j, c;
    c = 1;
    for(i=0; i<m; i++){
        for(j=0; j<n; j++){
            p = fork();
            if(!p)child_1();
        }
    }
    for(i=0; i<m; i++){
        for(j=0; j<n; j++){
            wait(NULL);
            c++;
        }
    }
    fprint(stout, "%d process created\n", c);
}

void child_2(int n){
    int i;
    for(i=0;i<n;i++){
        if(!fork()) child_1();
    }
    for(i=0; i<n; i++){
        wait(NULL);
        c++;
    }
    exit(c);
}

void multiple_fork_2(int n, int m){
    int i, c;
    c = 1;
    for(i=0; i<m; i++){
        if(!fork())child_2(n);
    }
    for(i=0; i<m; i++){
        wait (&s);
        c += WEXITSTATUS(s);
    }
    fprintf(stdout, "%d processes created\n", c);
}


int main(int argc, char **argv)
{
    //fork_test();
    //fork_pid_test();
    //fork_rand_test();

    //fork_sleep_and_wait();

    if(argc!=3){
        prinf("usage fork n m");
        return (0);
    }
    else {
        multiple_fork(atoi(argv[1])), atoi(argv[2])));
        return 0;
    }


    return 0;
}