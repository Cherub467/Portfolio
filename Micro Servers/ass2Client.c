/* 
 * Compile using "cc -o client client.c"
 */
 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_BUF_LENGTH 512

/* Hardcode the IP address of the bridge server */
#define SERVER_IP "127.0.0.1"

/* Edit as needed to match port of server */
#define SERVER_PORT 6733
    
int main(void){
	struct sockaddr_in si_server;
        struct sockaddr *server;
        int s, i, len = sizeof(si_server);
        char buf[MAX_BUF_LENGTH];
        int readBytes;
        int quit, swtNum, input;

//TCP socket creation-----------------------------------------------------------------
        memset((char *) &si_server, 0, sizeof(si_server));
        si_server.sin_family = AF_INET;
        si_server.sin_port = htons(SERVER_PORT);
	si_server.sin_addr.s_addr = htonl(INADDR_ANY);

	//socket
	int clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSock == -1) printf("socket() call failed");
quit = 0;
	//connection
	int status = connect(clientSock, (struct sockaddr*)&si_server, sizeof(struct sockaddr_in));
	if(status == -1) printf("connect() call failed");

//--------------------------------------------------------------------------------------
		printf("This is the client of Assignment 2\n");
while(quit == 0){
	        printf("Your choices are:\n1 Translate\n2 Currency Converter\n3 Voting\n4 quit\n");
//input------------------------
		printf("Enter a command by its number: ");
		scanf("%d", &input);
//switch----------------------
		char message[MAX_BUF_LENGTH];
		bzero(message, MAX_BUF_LENGTH);
		bzero(buf, MAX_BUF_LENGTH);
		switch(input){
			case 1:		//translate

				char trans[MAX_BUF_LENGTH];
                                bzero(trans, MAX_BUF_LENGTH);
				
				printf("hello, goodbye, seven, road, blue: ");
				scanf(" %s", trans);	//ask userinput for wor

				//start the messaage to send, 1 means translate service
				strcpy(message, "1 ");
				strcat(message, trans);

				printf("asked for translation of %s\n", trans);
				int test;
				if((test = send(clientSock, message, MAX_BUF_LENGTH, 0)) == -1){
                 	       		printf("send to failed translate\n");
                        		quit = 1;
                		}
						
				if (recv(clientSock, buf, MAX_BUF_LENGTH, 0)==-1){
                        		printf("Read error! translate\n");
                        		quit = 1;
                		}
				
				printf("The translation of %s in french is %s.\n", trans, buf);
				break;
			case 2:		//currency
			
				printf("type your conversion in this format: ### source destination\n");
                                char cnv[MAX_BUF_LENGTH], src[5], dest[5];
                                scanf(" %s %s %s", cnv, src, dest); //ask userinput for conversion

				strcpy(message, "2 ");     //start the messaage to send, 2 meaans conversion
                                strcat(message, cnv);
				strcat(message, " ");
				strcat(message, src);
				strcat(message, " ");
				strcat(message, dest);

                                if(send(clientSock, message, MAX_BUF_LENGTH, 0) == -1){
                                        printf("send to failed translate\n");
                                        return 1;
                                }
				
                                if (recv(clientSock, buf, MAX_BUF_LENGTH, 0)==-1){
                                        printf("Read error! translate\n");
                                        quit = 1;
                                }
                                printf("your conversion of ( %s ) was %s.\n", cnv, buf);
				break;
			case 3:	//voting
				char choice[MAX_BUF_LENGTH], vote[5];
				printf("What would you like to do:\n 1 Show canidates\n 2 Secure voting\n 3 Voting summary\n");
				scanf("%s", choice);

				strcpy(message, "3 ");     //start the messaage to send, 3 means voting
				strcat(message, choice);
				
			//canaadet list-------------------------------------------------
				if(choice[0] == '1'){
					printf("requesting canidate list\n");

					if(send(clientSock, message, MAX_BUF_LENGTH, 0) == -1){
                                        	printf("send to failed translate\n");
                                        	return 1;
                                	}

					if (recv(clientSock, buf, MAX_BUF_LENGTH, 0)==-1){
                                        	printf("Read error! translate\n");
                                        	quit = 1;
                                	}

					printf("%s\n", buf);
			//voting--------------------------------------------------------
				}else if(choice[0] == '2'){
					printf("getting voting informtion\n");
					
					if(send(clientSock, message, MAX_BUF_LENGTH, 0) == -1){
                                                printf("send to failed translate\n");
                                                return 1;
                                        }

					if (recv(clientSock, buf, MAX_BUF_LENGTH, 0)==-1){ //getting key from server
                                        	printf("Read error! translate\n");
                                        	quit = 1;
                                	}
					printf("enter your vote: ");
					bzero(message, MAX_BUF_LENGTH);
					scanf(" %s", vote);				//getting vote from user

					int secure = (int)vote[0] * (int)buf[0];	//encoding the vote
					sprintf(message, "%d", secure);
					
					if(send(clientSock, message, MAX_BUF_LENGTH, 0) == -1){
                                                printf("send to failed translate\n");
                                                return 1;
                                        }
					printf("Voting complete\n");
			//results-------------------------------------------------------
				}else if(choice[0] == '3'){
					printf("getting results\n");

                                        if(send(clientSock, message, MAX_BUF_LENGTH, 0) == -1){
                                                printf("send to failed translate\n");
                                                return 1;
                                        }

					if (recv(clientSock, buf, MAX_BUF_LENGTH, 0)==-1){ //getting key from server
                                                printf("Read error! translate\n");
                                                quit = 1;
                                        }
					printf("%s", buf);

				}else printf("Not a valid option\n");

				break;
			case 4:
					//quit
				if(send(clientSock, "quit", MAX_BUF_LENGTH, 0) == -1){
                                        printf("send to failed quit\n");
                                        return 1;
                                }
				quit = 1;
				break;
			default:
				printf("\nCommand not found please try again\n");
				continue;
			
		}
	}
	close(s);
	return 0;
	
}
