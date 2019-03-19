#include "ask1.h"


int down(int semid,int sem_number,int pid){
	struct sembuf sem_b;

	sem_b.sem_op = -1; /*down sem by 1*/
	sem_b.sem_num = sem_number;
	sem_b.sem_flg = 0;
    	if (semop(semid,&sem_b, 1) == -1) printf("Semaphore_down failed\n");
    	return 1;
}




int up(int semid,int sem_number,int pid){
	struct sembuf sem_b;

	sem_b.sem_op = 1; /*up sem by 1*/
	sem_b.sem_num = sem_number;
	sem_b.sem_flg = 0;
    	if (semop(semid,&sem_b, 1) == -1) printf("Semaphore_up failed\n");
        return 1;
}




int setSemToX(int semid,int value,int sem_number){
	union semun sem;
	sem.val = value;

	if(semctl(semid,sem_number, SETVAL,sem) == -1) fprintf(stderr, "Semaphore_set_value failed   \n");
	return 1;
}




int get_semaphore_value(int semid,int sem_number){
	union semun sem;

	int value = semctl(semid,sem_number,GETVAL,sem);
	return value;
}



void check(int argc,char ** argv,int *Y){

	if(argc != 2){
		perror("Failed with argument in command line \n");
		exit(EXIT_FAILURE);
	}
	if(atoi(argv[1]) < 1){
		perror("Failed with argument in command line \n");
                exit(EXIT_FAILURE);
	}
	*Y = atoi(argv[1]);
	return;
}



double current_time() {
    struct timeval te;

    gettimeofday(&te, NULL); 				    // get current time
    double milliseconds = te.tv_sec*1000 + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}



long long compute_number(int16_t a,int16_t b,int16_t c){
	long long number;

	number = a;
	number *= 10000;
	number += b;
	number *= 10000;
	number += c;

	return number;
}
