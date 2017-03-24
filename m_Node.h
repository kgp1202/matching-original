
#ifndef __H_M_DATA
#include "m_Command.h"
#endif

class Node{
private:
	Node* _next;
	Command* _command;
public:
	Node();
	Node(Command*);
	~Node();
	Node* next();
	void setNext(Node*);
	Command* getCommand();
};

Node::Node(){
	_next = NULL;
	_command = NULL;
}

Node::Node(Command* c){
	_next = NULL;
	_command = c;
}

Node* Node::next(){
	return _next;
}

void Node::setNext(Node* node){
	_next = node;
}

Command* Node::getCommand(){
	return _command;
}

