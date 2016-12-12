#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

int main(){
  //Create/get access to a semaphore.
  int semid = semget(ftok("makefile", 22), 1, IPC_CREAT | 0644);

  //Create or access a shared memory segment.
  int shmid = shmget( ftok("makefile", 22), 4, IPC_CREAT | 0644 );

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;

  semop(semid, &sb, 1);//down flag

  int* shmt = shmat(shmid, 0, 0);//Attach a shared memory segment to a variable
  int fd = open("file.txt", O_RDWR | O_APPEND);//open story file
  char * buff;//buffer

  //gets # of bytes the current position is from the beginning of the file
  int lastbytes = lseek(fd, -(*shmt), SEEK_END );//from end

  //read from the fd's file and put that data into buff
  read( fd, buff, lastbytes );

  if(!lastbytes) //new story
    printf("Create your story: ");
  else {
    printf("Last update: %s\n", buff );
    printf("Add yours here: " );
  }

  //gets input and writes and closes file
  char input[1024];
  fgets(input, sizeof(input), stdin);//gets input
  write(fd, input, sizeof(input));//writes
  close(fd);

  //sets length of update and detach
  *shmt = strlen(input);
  shmdt(shmt);

  //set semaphore back to 0
  sb.sem_op = 1;
  semop(semid, &sb, 1);

  return 0;
}
