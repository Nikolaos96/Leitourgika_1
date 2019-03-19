#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

union semun {
	int val;
	struct semid_ds *buffer;
	unsigned short *array;
	struct seminfo *__buffer;
};

/*
struct sembuf {
	short sem_num;
	short sem_op;
	short sem_flg;
};
*/

typedef struct mystruct{
	double time;
	int16_t part_number;
}mystruct;


/*****************************************************************************************/

int down(int,int,int);
int up(int,int,int);
int setSemToX(int,int,int);
int get_semaphore_value(int,int);
void check(int,char **,int *);
double current_time();
long long compute_number(int16_t,int16_t,int16_t);
