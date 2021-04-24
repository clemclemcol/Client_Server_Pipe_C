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

struct list_of_messages{
  char * mess;
  struct list_of_messages *next;
};

struct list_of_messages *db;
struct list_of_messages *current;

void add_message(char *s){
  struct list_of_messages *m = malloc(sizeof(struct list_of_messages));
  m->mess = strdup(s);
  m->next = NULL;
  struct list_of_messages *cur;
  struct list_of_messages *prev = NULL;
  cur = db;
  while (cur){
    prev = cur;
    cur = cur->next;
  }
  if (prev){
    prev->next = m;
  } else {
    db = m;
  }
}

void initialize_db(){
  add_message("Bonjour, ceci est un affichage informatisé");
  time_t t = time(NULL);
  struct tm *lt = localtime(&t);
  char timestring[18];
  sprintf(timestring, "%02d/%02d/%02d-%02d:%02d:%02d", lt->tm_mday, lt->tm_mon, lt->tm_year%100, lt->tm_hour, lt->tm_min, lt->tm_sec);
  free(tm);
  add_message(timestring);
  current = db;
}

void print_message(){
  if (!current){
    printf("no message available\n");
    return;
  }
  printf("%s\n", current->mess);
  if (current->next){
    current = current->next;
  } else {
    current = db;
  }
}

void delete_db(){
  struct list_of_messages *cur = db;
  struct list_of_messages *next;
  while (cur){
    next = cur->next;
    free(cur->mess);
    free(cur);
    cur = next;
  }
  db = NULL;
  current = NULL;
}

void reset_db(){
  delete_db();
  initialize_db();
}

void handler1(){
  printf("sys: resetting database\n");
  reset_db();
}

void handler2(){
  int fd, size;
  char buf[BUF_SIZE];
  printf("sys: adding message\n");
  fd = open(FIFO, O_RDONLY);
  read (fd, &size, sizeof(int));
  if (size > BUF_SIZE){
    fprintf(stderr, "message too big\n");
    return;
  }
  read(fd, buf, size);
  close(fd);
  add_message(buf);
}


int main(int argc, char **argv){
  pid_t p = getpid();
  int fd;
  
  mkfifo(FIFO, 0666);
  
  fd = open(PID, O_WRONLY|O_CREAT|O_TRUNC,00666);
  write(fd, &p, sizeof(pid_t));
  close(fd);
  
  struct sigaction s1,s2;
  s1.sa_handler = handler1;
  s1.sa_flags = SA_ONSTACK;
  sigaction(SIGUSR1, &s1, NULL);
  s2.sa_handler = handler2;
  s2.sa_flags = SA_ONSTACK;
  sigaction(SIGUSR2, &s2, NULL);
  initialize_db();
  
  while (1){
    sleep(2);
    print_message();
    //print_message();
  }
  return 0;
}
