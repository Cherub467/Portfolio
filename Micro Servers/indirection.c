/*assignment 2 indirection server
 * Lauren Mayes, 30062361
 * 
 * The indirection server connects to a client with a TCP connection
 * it then parses the messaage and sends it to a micro server on UDP
 * recevs the result from the micro server and sends it to the client
 * */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>


#define TCPPORT 6733
#define UDPPORT 6656
#define PORTONE	6740
#define PORTTWO 6741
#define PORTTHREE 6742

#define MAX_BUF_LENGTH 512 

#define TCP_IP "136.159.5.25"  /* csx1.cpsc.ucalgary.ca */
#define UDP_IP "136.159.5.27"  /* csx3.cpsc.ucalgary.ca */


int main(void){

	char inMessage[MAX_BUF_LENGTH], outMessage[MAX_BUF_LENGTH];
	int readBytes;
int quit = 0;
//while(!quit){	
//TCP socket creation------------------------------------------------------------------------------
	//address init
	struct sockaddr_in in_server;
	memset(&in_server, 0, sizeof(in_server));
	in_server.sin_family = AF_INET;
	in_server.sin_port = htons(TCPPORT);
	in_server.sin_addr.s_addr = htonl(INADDR_ANY);

	//socket creation
	int indiSock = socket(AF_INET, SOCK_STREAM, 0);
	if(indiSock == -1){
		printf("TCP socket creation faailed\n");
	}

	//Binding
	int status = bind(indiSock, (struct sockaddr *)&in_server, sizeof(struct sockaddr_in));
	if(status == -1){
		printf("TCP socket bind error\n");
	}
	
	//listening
	int listener = listen(indiSock, 5);
	if(listener == -1){
                printf("TCP socket listen error\n");
        }

	//accepting
	int indirection = accept(indiSock, NULL, NULL);
	if(indirection == -1){
                printf("TCP socket acception error\n");
        }
printf("TCP socket created\n");
while(quit == 0){
//recv msg from client--------------------------------------------------------------------------------
	int c;
	bzero(inMessage, MAX_BUF_LENGTH);
	bzero(outMessage, MAX_BUF_LENGTH);
	if((c = recv(indirection, inMessage, sizeof(inMessage), 0)) == -1){
		printf("read error client");
	}
	printf("recived ( %s ) from client\n", inMessage);

	struct sockaddr_in si_server;
	struct sockaddr *server;
	int s, i, len = sizeof(si_server);
	int svrLen = sizeof(si_server);
	
	switch(inMessage[0]){
//Traanslation micro server---------------------------------------------------------------------------
		case '1':

			if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) {
				printf("Could not setup a socket!\n");
				return 1;
			}
      
			memset((char *) &si_server, 0, sizeof(si_server));
			si_server.sin_family = AF_INET;
			si_server.sin_port = htons(PORTONE);
			server = (struct sockaddr *) &si_server;

			if (inet_pton(AF_INET, UDP_IP, &si_server.sin_addr)==0){
				printf("inet_pton() failed\n");
				return 1;
			}
			printf("UDP socket created\n");
		//send to micro server
			if(sendto(s, inMessage, strlen(inMessage), 0, server, sizeof(si_server)) == -1){
				printf("udp sentto failed translate\n");
				return 1;	
			}
			printf("send to trans microserver success\n");
		//recev from microserver
			
			if((readBytes = recvfrom(s, outMessage, MAX_BUF_LENGTH, 0, server, &svrLen)) <= 0){
					printf("Read error!\n");
					quit = 1;
			}
			
			if(send(indirection, outMessage, MAX_BUF_LENGTH,0)==-1){
				printf("send failed, tcp translate");
			}
			printf("recived %s from server, sent to client\n", outMessage);			
			break;	
//conversion micro server--------------------------------------------------------------------------------
		case '2':
                        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) {
                                printf("Could not setup a socket!\n");
                                return 1;
                        }

                        memset((char *) &si_server, 0, sizeof(si_server));
                        si_server.sin_family = AF_INET;
                        si_server.sin_port = htons(PORTTWO);
                        server = (struct sockaddr *) &si_server;

                        if (inet_pton(AF_INET, UDP_IP, &si_server.sin_addr)==0){
                                printf("inet_pton() failed\n");
                                return 1;
                        }
                        printf("UDP socket created\n");
                //send to micro server
                	if(sendto(s, inMessage, strlen(inMessage), 0, server, sizeof(si_server)) == -1){
                		printf("udp sentto failed conversion\n");
                		return 1;
                	}
                	printf("send to convert microserver success\n");
                //recev from microserver

                	if((readBytes = recvfrom(s, outMessage, MAX_BUF_LENGTH, 0, server, &svrLen)) <= 0){
                		printf("Read error!\n");
                		quit = 1;
                	}
                	
			if(send(indirection, outMessage, MAX_BUF_LENGTH,0)==-1){
                		printf("send failed, tcp conversion");
                	}
                	printf("recived %s from server, sent to client\n", outMessage);
                        break;
		 
		case '3':
			if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) {
                                printf("Could not setup a socket!\n");
                                return 1;
                        }

                        memset((char *) &si_server, 0, sizeof(si_server));
                        si_server.sin_family = AF_INET;
                        si_server.sin_port = htons(PORTTHREE);
                        server = (struct sockaddr *) &si_server;

                        if (inet_pton(AF_INET, UDP_IP, &si_server.sin_addr)==0){
                                printf("inet_pton() failed\n");
                                return 1;
                        }
                        printf("UDP socket created\n");
		//send to micro server
			if(sendto(s, inMessage, strlen(inMessage), 0, server, sizeof(si_server)) == -1){
                                printf("udp sentto failed voting\n");
                                return 1;
                        }
                        printf("send to voting microserver success\n");
		//recev from microserver
			if((readBytes = recvfrom(s, outMessage, MAX_BUF_LENGTH, 0, server, &svrLen)) <= 0){
                                printf("Read error!\n");
                                quit = 1;
                        }

                        if(send(indirection, outMessage, MAX_BUF_LENGTH,0)==-1){
                                printf("send failed, tcp voting");
                        }
                        printf("recived %s from server, sent to client\n", outMessage);
			break;

                default:
                        printf("fellthrough switch");
                        continue;
	}
}//end while loop
	close(indirection);
}//end main



