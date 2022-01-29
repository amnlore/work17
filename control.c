#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define SH_KEY 24602
#define SEM_KEY 24603

int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "-create") == 0) {
            int shmd = shmget(SH_KEY, sizeof(int), IPC_CREAT | 0640);

            int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);

            union semun us;
            us.val = 1;
            int r = semctl(semd, 0, SETVAL, us);

            char * file = argv[2];
            int f = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
            close(f);

            return 0;
       }

       else if (strcmp(argv[1], "-remove") == 0) {

           int shmid = shmget(SH_KEY, 0, 0);
           shmctl(shmid, IPC_RMID, 0);

           int semid = semget(SEM_KEY, 1 ,0);
           semctl(semid, IPC_RMID, 0);

           char story[1000];
           int f = open("file.txt", O_RDONLY);
           read(f, story, 1000);
           printf("Story: \n%s", story);

           close(f);
           return 0;
       }
       else {
           printf("Use -create or -remove\n");
       }
   }
   else {
       printf("Use -create or -remove\n");
   }

   return 0;
}
