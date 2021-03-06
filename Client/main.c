#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

#define answer_length 128
#define buf_length 1024
#define path_length 100
#define ip_length  16
#define out_length 100

char hello_message[] = "Hello";
char OK[] = "OK"; 
char message[] = "message";
char DONE[] = "___done___";
char output_file[out_length];
char answer[answer_length];
char buf[buf_length];
char path[path_length];
char serv_ip[ip_length];
FILE *pFile;

int main()
{
	int sock;
	int bytes_read;
	struct sockaddr_in addr;

	//creating socket
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock < 0)
	{
		perror("socket");
		exit(1);
	}

	//setting server ip
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3561);

	printf("server ip: ");	

	scanf("%s", serv_ip);
	if(inet_pton(AF_INET, serv_ip, &(addr.sin_addr)) <=0){	
		perror("server address:");	
		exit(1);
	}

	//connecting the socket
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){	
		perror("connect");
		exit(2);	
	}

	//exchanging hello_message
	send(sock, hello_message, sizeof(hello_message),0);
	if(recv(sock, answer, answer_length,0) <= 0){
		perror("hello message");
		exit(3);
	}
	printf("connection established \n");

	//requesting file path
	printf("file path: ");
	scanf("%s", path);

	//requesting output file path
	printf("output file path: ");
	scanf("%s", output_file);

	//transmitting file path
	send(sock, path, sizeof(path),0);

	//creating file to output the recieved data
	pFile = fopen(output_file, "wb");
	if(pFile == NULL) {
		perror("Write error");	
		exit(4);
	}else{
		printf("Created the file for output\n");
		do{
			bytes_read = recv(sock, buf, buf_length, 0);

			//checks if the server finished the transmition
			if(strcmp(buf, DONE) == 0) {
				printf("done...\n");
				break;
			}
			//check for message
			if(strcmp(buf, message) == 0){
				fclose(pFile);
				close(sock);
				printf("File not found, terminating...\n");
				exit(4);
			}

			//if it's not the case, write the hello_message to file
			if(fwrite(buf, 1, bytes_read, pFile) < bytes_read){
				send(sock, message, sizeof(message), 0);
				fclose(pFile);
				close(sock);
				perror("write error");
				exit(4);
			}
			//tell the server everything went fine
			send(sock, OK, sizeof(OK), 0);
		}while(bytes_read > 0);
	}

	fclose(pFile);
	close(sock);


	return 0;

}
