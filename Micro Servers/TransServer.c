/* Assignment 2, Lauren Mayes, 30062361
 * Translate Service server
 * Translates a given english word to french
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

//UDP port creation---------------------------------------------------------------------------
    struct sockaddr_in si_server, si_client;
    struct sockaddr *server, *client;
    int s, i, len=sizeof(si_server);
    int port = 6740;					// Port number
    char buf[MAX_BUF_LEN];
    char tosend[MAX_BUF_LEN];
    int readBytes;
    int choice;
int quit =0;   
    printf("Translate service on port %d\n", port);


    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not setup a socket!\n");
	return 1;
      }
	printf("socket created\n");
    
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
	printf("socket bind\n");
	
printf("socket connected successfully\n");
//----------------------------------------------------------------------------------------------	
while(quit == 0){
	bzero(tosend, MAX_BUF_LEN);
	bzero(buf, MAX_BUF_LEN);
	if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, client, &len))==-1)
	  {
	    printf("Read error!\n");
	    quit = 1;
	  }
	buf[readBytes] = '\0'; // padding with end of string symbol

	printf("Server received command \"%s\" from client %s on port %d\n",
	       buf, inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));
	char *temp;
	temp = strchr(buf,' ');
	printf("after tok %s\n", temp);
	if(strncmp(temp, "quit", 4) == 0) quit = 1;
	if( quit == 1 ) sprintf(tosend, "%s", "OK");

//english to french conversion------------------------------------------------------------------
	else if(strncmp(temp, " hello", 4) == 0){   
	    sprintf(tosend,"%s", "bonjour");
	}
	else if(strncmp(temp, " goodbye", 3) == 0){    
	    sprintf(tosend,"%s", "au revoir");
	}
	else if(strncmp(temp, " blue", 4) == 0){    
	    sprintf(tosend,"%s", "bleue");
	}
        else if(strncmp(temp, " seven", 4) == 0){
            sprintf(tosend,"%s", "sept");
        }
        else if(strncmp(temp, " road", 4) == 0){
            sprintf(tosend,"%s", "route");
        }
	else{    
	    sprintf(tosend,"%s", "word not found");
	}

	printf("Sending back \"%s\" as a response\n", tosend);
	
	if(sendto(s, tosend, MAX_BUF_LEN, 0, client, len) == -1){
		printf("send to failed");
	}
	printf("responce sent\n");		
      }
    close(s);
    return 0;
  }
