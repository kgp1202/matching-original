#include "m_DataStructure.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//Sharing value with thread-pool
CommandQueue* queue;
DataStructure* ds;

//const value
const int serv_port_num = 7002;
const int serv_listen_num = 1000;
const int thread_num = 5;

//worker thread's function
void* worker_func(void* ptr){
	while(1){
		Command* c = queue->pop();
		if(c == NULL){
			continue;
		}
		Data* data = (Data*)c->execute();
		ds->push(data);
	}
	return NULL;
}

int main(){
	//init queue
	queue = new CommandQueue();

	//init serv_sock
        int serv_sock;
        struct sockaddr_in serv_sock_addr;

        if((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                error("socket in main.c");
        }

        serv_sock_addr.sin_family = AF_INET;
        serv_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_sock_addr.sin_port = htons(serv_port_num);

        socklen_t serv_sock_len = sizeof(serv_sock_addr);
        if(bind(serv_sock, (struct sockaddr*)&serv_sock_addr,
                serv_sock_len) < 0){
                error("bind in main.c");
        }

        if(listen(serv_sock, serv_listen_num) < 0){
                error("listen in main.c");
        }

	//data structrue 
	ds = new DataStructure();

	//init worker thread-pool 
	pthread_t* tid = (pthread_t*) malloc(sizeof(pthread_t) * thread_num);
	
	for(int i = 0; i < thread_num; i++){
		pthread_create(&tid[i], NULL, &worker_func, NULL);
	}

	//accept loop
	int client_sock;
	struct sockaddr_in client_sock_addr;
	socklen_t client_sock_addr_len;
	while(true){
		 client_sock = accept(serv_sock,
                                (struct sockaddr*)&client_sock_addr,
                                &client_sock_addr_len);

                if(client_sock < 0){
                        error("accept in main.c");
                }

		//push to queue
		Command* pushedCommand = new RequestCommand(client_sock);
		queue->push(pushedCommand);
	}
}
