#include "m_CommandQueue.h"

class DataStructure{
private:
	Data* dataArr[5];
	CommandQueue* _handler;
	pthread_t _tid;
	
	//Data 처리 알고리즘.
	void* DataStructure_func(void* ptr){
		int count = 0;
		while(true){
			Data* data = (Data*)_handler->pop();

			if(data == NULL){
				continue;
			}
		
			dataArr[count % 4] = data;
			count++;
			if(count % 4 == 0){
				for(int i = 0; i < 4; i++){
					printf("%s\n", dataArr[i]->_str);
				}
			}
		}

		return NULL;	
	}
	static void* helper(void* context){
		return ((DataStructure *)context)->DataStructure_func(NULL);
	}
public:	
	DataStructure();
	void push(Data*);
};

DataStructure::DataStructure(){
	_handler = new CommandQueue();
	pthread_create(&_tid, NULL, &DataStructure::helper, this);
}

void DataStructure::push(Data* data){
	_handler->push(data);
}
