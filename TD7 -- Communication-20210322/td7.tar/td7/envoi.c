#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#define BUF_SIZE 255
#define FIFO "/tmp/fifo_affichage"
#define PID "/tmp/pid_affichage"


pid_t aff_pid(){
  struct stat s;
  if (stat(PID, &s) < 0) return -1;
  int fd = open(PID, O_RDONLY);
  if (fd < 0) return -1;
  pid_t p;
  read(fd, &p, sizeof(pid_t));
  close(fd);
  return p;
}

void reset(){
  pid_t p = aff_pid();
  if (p < 0){
    fprintf(stderr, "unable to get pid for affichage\n");
    exit(1);
  }
  kill(p, SIGUSR1);
}

void add_message(char *mess){
  pid_t p = aff_pid();
  if (p < 0){
    fprintf(stderr, "unable to get pid for affichage\n");
    exit(1);
  }
  int size = strlen(mess)+1;
  if (size > BUF_SIZE){
    fprintf(stderr, "message too long\n");
    exit(1);
  }
  kill(p, SIGUSR2);
  int fd;
  fd = open(FIFO, O_WRONLY);
  write(fd, &size, sizeof(int));
  write(fd, mess, size);
  close(fd);
}

int main(int argc, char **argv){
  if (argc < 2){
    printf("usage: envoi reset or envoi add [message]\n");
    return 0;
  }
  if (strcmp(argv[1],"reset") == 0){
    reset();
    return 0;
  }
  if (strcmp(argv[1], "add") == 0 && argc > 2){
    add_message(argv[2]);
    return 0;
  }
  printf("usage: envoi reset or envoi add [message]\n");
  return 0;
}
