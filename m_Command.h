#define __H_M_COMMAND

#ifndef __H_M_ERROR
#include "m_Error.h"
#endif

using namespace std;

class Command{
public:
	virtual void* execute() = 0;
};

//---------------------------------------------

class Data : public Command{
public:
	Data();
	char* _str;
	void* execute();
};

Data::Data(){

}

void* Data::execute(){
	return NULL;	
}

//---------------------------------------------

class RequestCommand : public Command{
private:
	int _fd;
public:
	RequestCommand(int);
	void* execute();
};

RequestCommand::RequestCommand(int fd){
	_fd = fd;
}

void* RequestCommand::execute(){
	//Data의 형태로 변환
	char buf[1024];
	read(_fd, buf, sizeof(buf));	

	Data* data = new Data();
	data->_str = buf;
	return data;
}

//---------------------------------------------

