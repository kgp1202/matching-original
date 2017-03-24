#include "m_Node.h"

#define __H_M_COMMANDQUEUE

struct sembuf sem_push_lock = {0, -2, SEM_UNDO};
struct sembuf sem_push_unlock = {0, 2, SEM_UNDO};
struct sembuf sem_pop_lock = {0, -3, SEM_UNDO};
struct sembuf sem_pop_unlock = {0, 3, SEM_UNDO};

class CommandQueue{
private:
	Node* _head;
	Node* _tail;	
	int _sem;
	bool _push_after_pop;
public:
	CommandQueue();
	int push(Command* );
	Command* pop();

	void show(){
		Node* temp = _head;
		while(temp != NULL){
			temp->getCommand()->execute();
			temp = temp->next();
		}
	}
};

CommandQueue::CommandQueue(){
	//init _sem
	if((_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666)) == -1){
		error("CommandQueue class, CommandQueue() semget");
	}
	if(semctl(_sem, 0, SETVAL, 3) == -1){
		error("CommandQueue class, CommandQueue() semctl");
	}

	_head = NULL;
	_tail = NULL;
	_push_after_pop = false;
}

int CommandQueue::push(Command* c){
	semop(_sem, &sem_push_lock, 1);

	Node* pushedNode = new Node(c);
	if(_tail == NULL){
		_head = pushedNode;
		_tail = pushedNode;

		if(_push_after_pop){
			_push_after_pop = false;
			semop(_sem, &sem_pop_unlock, 1);
			return 0;
		}	
	}else {
		_tail->setNext(pushedNode);
		_tail = pushedNode;
	}
	semop(_sem, &sem_push_unlock, 1);
	return 0;
}

Command* CommandQueue::pop(){
	semop(_sem, &sem_pop_lock, 1);

	Command* returnCommand;

	if(_head == NULL){
		_push_after_pop = true;
		semop(_sem, &sem_push_unlock, 1);
	
		return NULL;
	}
	else if(_head == _tail){
		returnCommand = _head->getCommand();
		_head = NULL;
		_tail = NULL;
		//delete _head;
	}
	else{
		returnCommand = _head->getCommand();
		_head = _head->next();	
		//delete _head;	
	}

	semop(_sem, &sem_pop_unlock, 1);
	return returnCommand;
}

