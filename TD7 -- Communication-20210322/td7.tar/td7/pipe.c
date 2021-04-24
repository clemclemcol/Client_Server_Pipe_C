#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>

#define BUF_SIZE 16


void write_chaine(int tuyau){
  char chaine[BUF_SIZE];
  strcpy(chaine, "coucou");
  printf("transmitting %s\n", chaine);
  write(tuyau, chaine, BUF_SIZE);
  close(tuyau);
  exit(0);
}

void q1(){
  int tuyau[2];
  pipe(tuyau);
  char *chaine = malloc(BUF_SIZE);
  if (!fork()) write_chaine(tuyau[1]);
  read(tuyau[0], chaine, BUF_SIZE);
  printf("received %s\n", chaine);
  close(tuyau[0]);
  wait(NULL);
}


void write_chaine_named(const char *pname){
  int fd;
  fd = open(pname, O_WRONLY);
  if (fd < 0){
    fprintf(stderr, "error opening write end of pipe %s\n", pname);
    exit(1);
  }
  write(fd, "Hello world", BUF_SIZE);
  close(fd);
  exit(0);
}

void q1_named(){
  char *pname = "/tmp/tuyau";
  int fd;
  char chaine[BUF_SIZE];
  if (mkfifo(pname, 0600) < 0){
    fprintf(stderr, "error creating pipe at %s\n", pname);
    return;
  }
  if (!fork()) write_chaine_named(pname);
  fd = open(pname, O_RDONLY);
  if (fd<0){
    fprintf(stderr, "error opening read end of pipe %s\n", pname);
    return;
  }
  read(fd, chaine, BUF_SIZE);
  close(fd);
  printf("received %s\n", chaine);
  wait(NULL);
  unlink(pname);
}








void write_ints_anon(int tuyau[]){
  close(tuyau[0]);
  srand(time(NULL));
  int n = rand();
  int m = rand();
  printf("transmitting %d %d\n", n, m);
  write(tuyau[1], &n, sizeof(int));
  write(tuyau[1], &m, sizeof(int));
  close(tuyau[1]);
  exit(0);
}

void q2_anon(){
  int tuyau[2];
  if (pipe(tuyau)<0){
    exit(1);
  }
  if (!fork()) write_ints_anon(tuyau);
  int o,p;
  close(tuyau[1]);
  read(tuyau[0], &o, sizeof(int));
  read(tuyau[0], &p, sizeof(int));
  close(tuyau[0]);
  printf("received %d %d\n", o, p);
  wait(NULL);
}
















void write_ints(char *pname){
  int fd = open(pname, O_WRONLY);
  srand(time(NULL));
  int n = rand();
  int m = rand();
  printf("transmitting %d %d\n", n, m);
  write(fd, &n, sizeof(int));
  write(fd, &m, sizeof(int));
  close(fd);
  exit(0);
}

void q2(){
  char *pname = "/tmp/tuyau";
  int o, p, fd;
  mkfifo(pname, 0666);
  if (!fork()) write_ints(pname);
  fd = open(pname, O_RDONLY);
  read(fd, &o, sizeof(int));
  read(fd, &p, sizeof(int));
  close(fd);
  printf("received %d %d\n", o, p);
  wait(NULL);
  unlink(pname);
}



int main(int argc, char **argv){
  //q1_named();
  //q2();
  q2_anon();
  return 0;
}
