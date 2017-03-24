#include "m_CommandQueue.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

CommandQueue* queue;

class TestCommand : public Command{
public:	
	int a;
	TestCommand(int k){
		a = k;
	}
	void* execute(){
		printf("%d\n", a);
	}
};

void* push(void* p){
	for(int i = 0; i < 10; i++){
		//printf("push %d\n", i);
		queue->push(new TestCommand(i));
	}
}

void* pop(void* p){
	while(true){
		//printf("pop\n");
		Command* c = queue->pop();
		if(c!=NULL){
			c->execute();
		}
	}
}
int main(){
	queue = new CommandQueue();

	pthread_t tid[10];
	for(int i = 0; i < 5; i++)
		pthread_create(&tid[i], NULL, &push, NULL);

	for(int i = 5; i < 10; i++)
		pthread_create(&tid[i], NULL, &pop, NULL);


	pthread_join(tid[0], NULL);
	sleep(10);
}
