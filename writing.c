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

int main() {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;

    int semd = semget(SEM_KEY, 1, 0);
    semop(semd, &sb, 1);
    int shmd = shmget(SH_KEY, 0, 0);
    int *size = shmat(shmd, 0, 0);
    int f = open("file.txt", O_RDWR | O_APPEND);
    lseek(f, -1 * (*size), SEEK_END);
    char last[*size];
    read(f, last, (*size)-1);
    printf("last line is: %s\n", last);
    printf("enter input: ");
    char input[1000];
    fgets(input, 1000, stdin);
    *size = strlen(input);
    write(f, input, strlen(input));
    shmdt(size);

    close(f);

    sb.sem_op = 1;
    semop(semd, &sb, 1);

    return 0;
 }
