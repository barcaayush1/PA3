
#ifndef _MQreqchannel_H_
#define _MQreqchannel_H_

#include "common.h"
#include "Reqchannel.h"
#include <mqueue.h>
#include <fcntl.h>

class MQRequestChannel: public RequestChannel
{
private:
	mqd_t wfd;
	mqd_t rfd;

	string s1;
	string s2;
	int open_ipc(string name, int mode);

public:
	MQRequestChannel(const string _name, const Side _side);
	
	~MQRequestChannel();

	int cread (void* msgbuf, int bufcapacity);

	int cwrite (void *msgbuf , int msglen);
	
	

};

#endif