#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

//Author: Lauren Mayes, 30062361
//Date: jan 29, 2021
//Course: CPSC 359
//Assignment 1

struct shared {
	int *progress;
	int *rank;
	int raceFinished;
	int *finishTime;
	int fl;
}var;

int initFinishLine(){
	
	int numHorse;
	while(1){
		printf("Enter a race length greater than 1: ");
		scanf("%d",&numHorse);
		if(numHorse >= 1) return numHorse;
	}

}

void *horse(void* q){
	int time, rng;
	time = 0;
        for(int i = 0; i < var.fl; i++){
                rng = (rand() % 3) + 1;
                sleep(rng);//sleep for 1-3 seconds
                var.progress[*(int*) q]++;
                time += rng;
	        }
        var.finishTime[*(int*) q] = time;
}


int initHorseThreads(){
	
	int numHorse;
	while(1){
		printf("Enter the amount of hourses, greater than 1: ");
		scanf("%d", &numHorse);
		if(numHorse >= 1) break;
	}

	var.progress = (int*)malloc(numHorse *sizeof(int));
	var.rank = (int*)malloc(numHorse * sizeof(int));
	var.finishTime = (int*)malloc(numHorse *sizeof(int));

	for(int i = 0; i < numHorse; i++){
		var.progress[i] = 0;
		var.rank[i] = -1;
		var.finishTime[i] = -1;	
	}
	return numHorse;	
}

void display(int h){
	for(int i = 0; i < h; i++){ //for every horse
		for(int j = 0; j < var.progress[i]; j++){
			printf("-");
		} 
		printf("%d \n", i+1);
	} 
}

int if_full(int h){
	int isFull;
	for(int i = 0; i < h; i++){
		if(var.finishTime[i] == -1) return isFull = 0;
	}
	return isFull = 1;
}

void main(){
	srand(time(0));//inti RNG

	var.fl = initFinishLine();
	int horses = initHorseThreads();

	pthread_t tid[horses];

	int *name = (int*)malloc(horses * sizeof(int));
	int *ptr;
        for(int i = 0; i < horses; i++){
	        name[i] = i;
		ptr = &name[i];
		pthread_create(&tid[i], NULL, horse, ptr);
	 }	
	
	while(var.raceFinished == 0){
		sleep(1);
		system("clear");
		display(horses);

		for(int i = 0; i < horses; i++){//update rank
	                if(var.finishTime[i] > -1){//if they have finished
	                        for(int j = 0; j < horses; j++) {//find the next avaliable rank
	                                if(var.rank[j] == -1){
					       	var.rank[j] = i+1; //rank the horse
						var.finishTime[i] = -2;
						break;
					}
				}
                        }
                }

		if(if_full(horses) == 1) break;
	}

	for(int i = 0; i < horses; i++){//join all threads
		pthread_join(tid[i], NULL);
	}

	for(int i = 0; i < horses; i++){//update rank
		if(var.finishTime[i] != -1){//if they have finished
			for(int j = 0; j < horses; j++) {//find the next avaliable rank
				if(var.rank[j] == -1) var.rank[j] = i; //rank the horse
				}
			}
                }

	printf("The final horse rankings are: ");
	for(int i = 0; i < horses; i++){
		printf("%d, ", var.rank[i]);
	}
	printf("\n");
	exit(0);
}
