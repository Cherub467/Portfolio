/* Assignment 2, Lauren Mayes, 30062361
 * Currancy conversion server
 * converts CAD to USD, euro,pound, and bitcoin
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

#define MAX_BUF_LEN 512
#define PORTRANGE_MIN 8000
#define MAX_LENGTH 10

int main(void){

//TCP port creation---------------------------------------------------------------------------
    struct sockaddr_in si_server, si_client;
    struct sockaddr *server, *client;
    int s, i, len=sizeof(si_server);
    int port = 6741;					// Port number
    char buf[MAX_BUF_LEN];
    char tosend[MAX_BUF_LEN];
    int readBytes;
    int choice;
   
//conversion rates--------------------------------------------------------------------------
	double toUSD = 0.81;
	double toEURO = 0.70;
	double toPOUND = 0.59;
	double toBTC = 0.000013;

    printf("Currancy conversion service on port %d\n", port);

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
printf("UDP socket created\n");
//----------------------------------------------------------------------------------------------	
    int quit = 0;
    while (!quit)
      {
	if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, client, &len))==-1)
	  {
	    printf("Read error!\n");
	    quit = 1;
	  }
	buf[readBytes] = '\0'; // padding with end of string symbol

	printf("  Server received command \"%s\" from client %s on port %d\n",
	       buf, inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));
	
	if(strncmp(buf, "quit", 4) == 0) quit = 1;
	if( quit == 1 ) sprintf(tosend, "%s", "OK");

//currancy conversion------------------------------------------------------------------
	char *token, *strtodble;
	double org;
	char dest[MAX_BUF_LEN], temp[MAX_BUF_LEN];
	bzero(dest, MAX_BUF_LEN);
	bzero(temp, MAX_BUF_LEN);

	token = strtok(buf, " ");		//token the sting, looking at switch num
	token = strtok(NULL," ");		//pass to org num
	strcpy(temp, token);
	org = strtod(temp, &strtodble);		//convert org num into double
	token = strtok(NULL, " ");		//move from number look at source
	token = strtok(NULL, " ");		//ignore source, look at destination
	strcpy(dest, token);			//copy destination into on string

	printf("Recived %f to convert to %s.\n", org,dest);

	if(strncmp(dest, "USD", 3) == 0){ 
		org = org * toUSD;
		sprintf(tosend, "%f", org);
	}
	else if(strncmp(dest, "EURO", 4) == 0){    
		org = org * toEURO;
		sprintf(tosend,"%f", org);
	}
	else if(strncmp(dest, "POUND", 5) == 0){    
		org = org * toPOUND;
		sprintf(tosend,"%f", org);
	}
        else if(strncmp(dest, "BTC", 3) == 0){
        	org = org * toBTC;
		sprintf(tosend,"%f",org);
        }
        else{    
	    sprintf(tosend, "destination not found");
	}

	printf("Sending back \"%s\" as a response\n", tosend);
	
	sendto(s, tosend, strlen(tosend), 0, client, len);		
      }
    close(s);
    return 0;
  }
