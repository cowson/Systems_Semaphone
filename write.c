#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>

int main(){
  srand(time(NULL));
  int x = random() % 10;
  int semid = semget(ftok("makefile", 22), 1, 0);
  printf("[%d] Before access, sleep: %x\n", getpid(), x);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;

  semop(semid, &sb, 1);
  printf("[%d] I'm in!\n", getpid());
  sleep(x);

  sb.sem_op =1;
  semop(semid, &sb, 1);
  printf("[%d] I'm done!\n", getpid());

  return 0;
}
