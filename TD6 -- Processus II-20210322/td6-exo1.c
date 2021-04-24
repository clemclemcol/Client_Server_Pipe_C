#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/times.h>

#define HOME "//mnt/c/Users/clemc/OneDrive/Bureau/UVSQ"

void fils_ps(){
    execlp("ps", "ps", "-A", NULL);
}

void fils_ls(){
    execlp("ls", "ls", "-R", HOME, NULL);
}

void fils_find_dir(int i){
    char * args[4];
    args[0] = "find";
    args[1] = HOME;
    args[2] = "-maxdepth";
    args[3] = "1";
    args[4] = "-type";
    args[5] = i ?"d": "f";
    args[6] = NULL;
    //execlp("find", "find", HOME, "-maxdepth 1", "-type d");
    execvp("find", args);
    
}

void fork_test(){
    pid_t p; //q
    int r;
    p = fork();
    if(!p){
        fils_find_dir(0);
        //q = wait(NULL);
    }/*else {
        fils_ps();
        fils_ls();
    }*/
    wait(NULL);
    printf("\nListing directories:\n");
    if (!fork()) fils_find_dir(1);
    wait(NULL);
}

void time_test(){
    struct tms horloge;
    if(!fork()){
        fils_ls();
    }
    wait(NULL);
    times(&horloge);
    float t;
    long d;
    d = horloge.tms_cutime + horloge.tms_cstime;
    t = ((float) horloge.tms_cutime + (float)horloge.tms_cstime)/((float)sysconf(_SC_CLK_TCK));
    fprintf(stdout, "temps ecoule %f, %ld cycles \n", t, d);
}

void fils_qui_compte(){
    for(int i = 1, i < 6; i++){
        sleep(1);
        printf("%d\n", i);
    }
    exit(0);
}

void stop_test(){
    pid_t p;
    p = fork();
    if (!p) fils_qui compte();
    sleep(3);
    kill(p, SIGSTOP);
    sleep(5);
    kill(p, SIGCONT);
    wait(NULL);

}


int main(int argc, char **argv)
{
    //fork_test();
    //time_test();
    
    return 0;
}