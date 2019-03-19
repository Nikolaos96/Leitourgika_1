/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "ask1.h"
#include "lista.h"


#define NUMBER_OF_PROCESS  7
#define MAX 9999
#define MIN 1000


int main(int argc, char **argv){

	int wpid ,status = 0, i, pid, Y;
	check(argc, argv, &Y);	// check the argument in command line

	/******	Creating semaphore array and shared memory space ********************/
	int keyForSharedMemSpace = 11111;
	int sm_id;	/* Shared memory for communication manufacturers - dyers and dyers - controls */
	if((sm_id = shmget(keyForSharedMemSpace, sizeof(mystruct), 0666 | IPC_CREAT)) == -1){
		fprintf(stderr, "Failed shared memory (sm_id) \n");
		exit(EXIT_FAILURE);
	}

	int keyForSharedMemSpace2 = 22222;
	int sm_id_2;	/* Shared memory for communication controls - fitting */
	if((sm_id_2 = shmget(keyForSharedMemSpace2, 3 * sizeof(mystruct), 0666 | IPC_CREAT)) == -1){
		fprintf(stderr, "Failed shared memory (sm_id_2 )\n");
		exit(EXIT_FAILURE);
	}

	int keyForSemaphore = 33333;
	int sem_id;	/* Semaphore set */
	if((sem_id = semget((key_t)keyForSemaphore, 13, IPC_CREAT | 0666)) == -1){
		fprintf(stderr, "Failed semaphores  \n");
		exit(EXIT_FAILURE);
	}


	/* Initialization of all the semaphores I created */
	/* semaphores for shared memory manufacturers - dyers and dyers - controls */
	setSemToX(sem_id,1,0);
	setSemToX(sem_id,0,1);
	setSemToX(sem_id,0,2);
	setSemToX(sem_id,0,3);
	setSemToX(sem_id,0,4);
	setSemToX(sem_id,0,5);

	/* semaphores for shared memory controls - fitting */
	setSemToX(sem_id,1,6);
	setSemToX(sem_id,0,7);
	setSemToX(sem_id,1,8);
	setSemToX(sem_id,0,9);
	setSemToX(sem_id,1,10);
	setSemToX(sem_id,0,11);
	setSemToX(sem_id,1,12);

	mystruct *mptr = (mystruct*) shmat(sm_id, (void *)0, 0);
	mystruct *kptr = (mystruct*) shmat(sm_id_2, (void *)0, 0);


	/* production of child processes */
	for(i = 0 ; i < NUMBER_OF_PROCESS ; i++){
		pid = fork();
		if(pid) pid = 0;
		else{
			pid = i+1;
			break;
		}
	}


	if(pid == 1 || pid == 2 || pid == 3){ /**********************    manufacturer  process     *****************/
		mystruct* mptr = (mystruct *)shmat(sm_id,(void*)0,0);
		if(pid == 1){	  // first manufacturer
			srand(1); // for random numbers
			for(i = 0 ; i < Y ; i++){
				down(sem_id,0,pid);
				down(sem_id,12,pid);
				double after = current_time();		      	      // current time in msec
				mptr->part_number = (rand() % (MAX + 1 - MIN)) + MIN; // random four-digit number
				mptr->time = after;				      // take current time
				up(sem_id,12,pid);
				up(sem_id,3,pid);
				up(sem_id,1,pid);
			}
		}else if(pid == 2){ // second manufacturer
			srand(2);
			for(i = 0 ; i < Y ; i++){
				down(sem_id,0,pid);
				down(sem_id,12,pid);
                                double after = current_time();
				mptr->part_number = (rand() % (MAX + 1 - MIN)) + MIN;
				mptr->time = after;
				up(sem_id,12,pid);
				up(sem_id,4,pid);
				up(sem_id,1,pid);
			}
		}else{		// third manufacturer
			srand(3);
			for(i = 0 ; i < Y ; i++){
				down(sem_id,0,pid);
				down(sem_id,12,pid);
                                double after = current_time();
                                mptr->part_number = (rand() % (MAX + 1 - MIN)) + MIN;
                                mptr->time = after;
				up(sem_id,12,pid);
				up(sem_id,5,pid);
				up(sem_id,1,pid);
			}
		}
		shmdt(mptr);
		return 0;
	}else if(pid == 4 || pid == 5 || pid == 6){/**********  process control    ************************************************************/
		mystruct *mptr = (mystruct *)shmat(sm_id, (void *)0, 0);
		mystruct *kptr = (mystruct *)shmat(sm_id_2, (void *)0, 0);
		info_deikti list;	// linked list for storage parts
        	list = NULL;
        	list = LIST_dimiourgia(&list);
		if(pid == 4){ // first control
			int b;
			double  c;
			for(i = 0 ; i < Y ; i++){
				down(sem_id,3,pid);
				down(sem_id,2,pid);
				down(sem_id,12,pid);
				b = mptr->part_number;
				c = mptr->time;
				usleep(100);
				// give the part in shared memory for fitting
				if(get_semaphore_value(sem_id,6) == 1){
					down(sem_id,6,pid);
					kptr[0].part_number = b;
					kptr[0].time = c;
					up(sem_id,7,pid);
				}else{ // store the part
					eisagogi(&list,b,c);
				}
				up(sem_id,12,pid);
				up(sem_id,0,pid);
			}
			/* give the parts you store */
			for(i = 1 ; i <= size_of_list(&list) ; i++){
				down(sem_id,6,pid);
				take_product(&list,i,&b,&c);
				kptr[0].part_number = b;
				kptr[0].time = c;
				up(sem_id,7,pid);
			}
		}else if(pid == 5){ // second control
			int  b;
                        double  c;
			for(i = 0 ; i < Y ; i++){
				down(sem_id,4,pid);
				down(sem_id,2,pid);
				down(sem_id,12,pid);
                                b = mptr->part_number;
                                c = mptr->time;
                                usleep(200);
                                if(get_semaphore_value(sem_id,8) == 1){
                                        down(sem_id,8,pid);
                                        kptr[1].part_number = b;
                                        kptr[1].time = c;
                                        up(sem_id,9,pid);
                                }else{
                        		eisagogi(&list,b,c);
                                }
				up(sem_id,12,pid);
				up(sem_id,0,pid);
			}
			for(i = 1 ; i <= size_of_list(&list) ; i++){
                                down(sem_id,8,pid);
				take_product(&list,i,&b,&c);
                                kptr[1].part_number = b;
                                kptr[1].time = c;
                                up(sem_id,9,pid);
                        }
		}else{		// third control
			int  b;
                        double  c;
			for(i = 0 ; i < Y ; i++){
                                down(sem_id,5,pid);
				down(sem_id,2,pid);
				down(sem_id,12,pid);
                                b = mptr->part_number;
                                c = mptr->time;
                                usleep(300);
                                if(get_semaphore_value(sem_id,10) == 1){
                                        down(sem_id,10,pid);
                                        kptr[2].part_number = b;
                                        kptr[2].time = c;
                                        up(sem_id,11,pid);
                                }else{
                        		eisagogi(&list,b,c);
                                }
				up(sem_id,12,pid);
                                up(sem_id,0,pid);
                        }
			for(i = 1 ; i <= size_of_list(&list) ; i++){
                                down(sem_id,10,pid);
                        	take_product(&list,i,&b,&c);
                                kptr[2].part_number = b;
                                kptr[2].time = c;
                                up(sem_id,11,pid);
                        }
		}
		lista_diagrafi(&list);
		shmdt(mptr);
		shmdt(kptr);
		return 0;
	}else if(pid == 7){ /**********************************    dyer process  *******************************/
		mystruct* mptr = (mystruct *)shmat(sm_id,(void*)0,0);
		//int b;
		double total_time = 0.0;
		for(i = 0 ; i < 3*Y ; i++){
			down(sem_id,1,pid);
			down(sem_id,12,pid);
			double before = mptr->time;
			double after = current_time();
			total_time += after - before;
			//b = mptr->part_number;
			usleep(400);
			//mptr->part_number = b;
			up(sem_id,12,pid);
			up(sem_id,2,pid);
		}
		total_time /= (3*Y);
		printf("Average time for painting is : %lf msec.\n", total_time);
                shmdt(mptr);
		return 0;
	}else if(pid == 0){ /****************************************  fitting  process   ***********************/
		mystruct *kptr = shmat(sm_id_2, (void *)0, 0);
		int16_t b1,b2,b3;
		double c, av_time = 0.0;
		long long tel_proion_b;
		long long *proionta = malloc(Y * sizeof(long long)); // array for storage all new product
                if(proionta == NULL){
                        fprintf(stderr, "Failed malloc proionta \n");
                        exit(EXIT_FAILURE);
                }
		for(i = 0 ; i < Y ; i++){
			down(sem_id,7,pid);
			b1 = kptr[0].part_number; 		 // take part type 1
			c = kptr[0].time;
			up(sem_id,6,pid);

			down(sem_id,9,pid);
			b2 = kptr[1].part_number; 		 // take part type 2
			if(kptr[1].time < c) c = kptr[1].time;
			up(sem_id,8,pid);

			down(sem_id,11,pid);
			b3 = kptr[2].part_number; 		 // take part type 3
			if(kptr[2].time < c) c = kptr[2].time;
			up(sem_id,10,pid);

			usleep(500);
			c = current_time() - c;
			tel_proion_b = compute_number(b1,b2,b3); // fitting new product
			proionta[i] = tel_proion_b;		 // store new product in a array
			av_time += c;				 // for average time product
		}
		av_time /= Y;
                printf("Average time for each product is : %.4lf msec. \n\n", av_time);
		for(i = 0 ; i < Y ; i++) printf("New Product with part_number  %lld \n", proionta[i]);
		free(proionta);
	}else{
		fprintf(stderr, "Fail with process \n");
		exit(EXIT_FAILURE);
	}

	while ((wpid = wait(&status)) > 0); // parent process wait all childs
	shmdt(mptr);
	shmdt(kptr);
        semctl(sem_id, 0, IPC_RMID);//remove semaphore set

	return 0;
}
/* End main */
