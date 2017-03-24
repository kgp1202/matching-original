#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void client_printf(char* msg){
	printf("client : %s\n", msg);
}

void c_error(char* msg){
	printf("client : %s\n", msg);
	exit(0);
}

char* serv_addr = "127.0.0.1";
short serv_port_number = 7002;

int main(){
	int nread;
	int request_sock;
	char buf[100];
	struct sockaddr_in request_sock_addr;	
	
	for(int i = 0; i < 10; i++){
		if((request_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			c_error("SOCKET");	
		}

		memset(&request_sock_addr, 0, sizeof(request_sock_addr));
		request_sock_addr.sin_family = AF_INET;
		request_sock_addr.sin_addr.s_addr = inet_addr(serv_addr);
		request_sock_addr.sin_port = htons(serv_port_number);
	
		printf("Try connect to server\n");

		if(connect(request_sock, (struct sockaddr*)&request_sock_addr,
			 sizeof(request_sock_addr)) < 0){
			perror("connect");
			c_error("connect");
		}

		char* buf2 = "hello server!";
		write(request_sock, buf2, strlen(buf2));	
		//write(request_sock, (void*)&i, sizeof(int));
	
		close(request_sock);

		client_printf("connection end\n");
	}
	return 0;
}
