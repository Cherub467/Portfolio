#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//Author: Lauren Mayes, 30062361
//Date: Jan 29, 2021
//Course: CPSC 359
//Assignment 1

struct shared {
	char board[9];
	char turn;
	char lastTurn;
} var;

void board(){

	for(int i = 0; i < 9; i++) {
		var.board[i] = '-';
	}

}

void display(){

	printf(" %c | %c | %c \n", var.board[0], var.board[1], var.board[2]);
	printf("----------------\n");
	printf(" %c | %c | %c \n", var.board[3], var.board[4], var.board[5]);
	printf("----------------\n");
	printf(" %c | %c | %c \n \n", var.board[6], var.board[7], var.board[8]);

}

char is_winner(){

	//check horizontal
	for(int i = 0; i < 9; i+=3){
		if(var.board[i] == var.board[i+1] && var.board[i] == var.board[i+2] && var.board[i+2] != '-') return var.board[i];
	}
	//check vertical
	for(int i = 0; i < 3; i++){
		if(var.board[i] == var.board[i+3] && var.board[i] == var.board[i+6] && var.board[i+6] != '-') return var.board[i];
	}
	//check diagonal /
	if(var.board[2] == var.board[4] && var.board[6] == var.board[2] && var.board[6] == 'x') return 'x';
	if(var.board[2] == var.board[4] && var.board[2] == var.board[6] && var.board[6] == 'o') return 'o';
	//check diagonal \  //
	if(var.board[0] == var.board[4] && var.board[0] == var.board[8] && var.board[8] == 'x') return 'x';
	if(var.board[0] == var.board[4] && var.board[0] == var.board[8] && var.board[8] == 'o') return 'o';
	
	return '-';
}

int board_full(){
	for(int i = 0; i < 9; i += 1){
		if(var.board[i] == '-') return 0;
	} return 1;
}

int move(){
	while(1){
                int move = rand() % 9;
                if(var.board[move] == '-') return move;
	 }
}


void *player(void* p){
	while(1){

	while(var.turn != *(char*) p){};

	var.board[move()] = *(char*) p;

	var.lastTurn = *(char*) p;
	var.turn = '-';
	}
}

void *trueMain(void* m){
	
	board();
	var.turn = '-';
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, player, "x");
	pthread_create(&tid2, NULL, player, "o");
	srand(time(0)); //init random number generator

	while(1){

	while(var.turn != '-'){};
	display();
	if(is_winner() != '-'){
		
		pthread_cancel(tid1);
		pthread_cancel(tid2);
		
		char winner = is_winner();
		printf("The winner is %c \n", winner);
		exit(0);
	}
	if(board_full() == 1){

		pthread_cancel(tid1);
		pthread_cancel(tid2);

		printf("The board is full, game ends in a tie. \n");		
		exit(0);
	}	
	else 
		if(var.lastTurn == 'x') var.turn = 'o';
       		else var.turn = 'x';
	}	
}

int main(){
	pthread_t tMain;
	pthread_create(&tMain, NULL, trueMain, NULL);
	pthread_join(tMain, NULL);
}
