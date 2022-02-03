/* Assignment 2, Lauren Mayes, 30062361
 *
 * Voting server
 * Send alist of candidates and voting key
 * recives ID * voting key and adds vote to tally
 */
 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <stdbool.h>

#define MAX_BUF_LEN 512
#define PORTRANGE_MIN 8000
#define MAX_LENGTH 10

int main(void){

//TCP port creation---------------------------------------------------------------------------
    struct sockaddr_in si_server, si_client;
    struct sockaddr *server, *client;
    int s, i, len=sizeof(si_server);
    int port = 6742;					// Port number
    char buf[MAX_BUF_LEN];
    char tosend[MAX_BUF_LEN];
    int readBytes;
    int choice;

    printf("Voting service on port %d\n", port);

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not setup a socket!\n");
	return 1;
      }
    
    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(port);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);
    server = (struct sockaddr *) &si_server;
    client = (struct sockaddr *) &si_client;
    if (bind(s, server, sizeof(si_server))==-1)
      {
	printf("Could not bind to port %d!\n", port);
	return 1;
      }

printf("socket bound\n");
//candidate creation-----------------------------------------------------------------------------
int numVote[4][2] = {{1,20},{2,30},{3,15},{4,19}};
char list[4][10] = {"Pikachu","Charizard","Venusar","Blastoise"};
//recives request-------------------------------------------------------------------------------	
    int quit = 0;
    while (!quit)
      {
	if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, client, &len))==-1)
	  {
	    printf("Read error!\n");
	    quit = 1;
	  }
	buf[readBytes] = '\0'; // padding with end of string symbol

	printf("Server received command \"%s\" from client %s on port %d\n",
	       buf, inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));
	
	if(strncmp(buf, "quit", 4) == 0)quit = 1;
	if( quit == 1 ) sprintf(tosend, "%s", "OK");

//english to french conversion------------------------------------------------------------------
	
	else if(strncmp(buf, "3 1", 3) == 0){   
		strcpy(tosend, "The #1 pokemon is: \n1 Picachu\n2 Charizard\n3 Venusar\n4 Blastoise.");   
	}
	else if(strncmp(buf, "3 2", 3) == 0){
		int secure = 7;			//secure key is 7, did not implement random
		sprintf(tosend, "%d", secure);	
		sendto(s, tosend, strlen(tosend), 0, client, len);//send key to client

		bzero(tosend, MAX_BUF_LEN);
		bzero(buf, MAX_BUF_LEN);

		//read encrypted responce from client
		if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, client, &len))==-1){
            		printf("Read error!\n");
            		quit = 1;
          	}

		secure = atoi(buf) / 7;		//decrypt vote
    
		switch(secure) {			//vote is the result of the secure key
			case 1:
				numVote[0][1]++;
				break;
			
			case 2:
				numVote[1][1]++;
				break;
			
			case 3:
				numVote[2][1]++;
				break;
			
			case 4:
				numVote[3][1]++;
				break;
			
		}
		sprintf(tosend, "%s", "voting complete");
	}
	else if(strncmp(buf, "3 3", 3) == 0){    
		int bigNum[] = {0,0};
		printf("before for loop\n");
		for(int i = 0; i<4; i++){
			if(numVote[i][1] > bigNum[1]){
				bigNum[0] = numVote[i][0];
				bigNum[1] = numVote[i][1];
			}
		printf("aafter for loop\n");
		sprintf(tosend, "%s is the winner with %d votes\n",list[bigNum[0]], bigNum[1]); 
		}
	}
        else{    
	    sprintf(tosend, "action not found");
	}

	printf("Sending back \"%s\" as a response\n", tosend);
	
	sendto(s, tosend, strlen(tosend), 0, client, len);		
      }
    close(s);
    return 0;
  }
