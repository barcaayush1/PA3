#include "common.h"
#include "MQreqchannel.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

MQRequestChannel::MQRequestChannel(const string _name, const Side _side) : RequestChannel (_name, _side){
	s1 = "/MQ_" + my_name + "1";
	s2 = "/MQ_" + my_name + "2";
	
	if (_side == SERVER_SIDE){
		wfd = open_ipc(s1, O_CREAT|O_WRONLY);
		rfd = open_ipc(s2, O_CREAT|O_RDONLY);
	}
	else{
		wfd = open_ipc(s2, O_CREAT|O_WRONLY);
		rfd = open_ipc(s1, O_CREAT|O_RDONLY);
	}
	
}

MQRequestChannel::~MQRequestChannel(){ 
	mq_close(wfd);
	mq_unlink (s1.c_str());
	mq_close(rfd);
	mq_unlink (s2.c_str());
}

int MQRequestChannel::open_ipc(string _pipe_name, int mode){
	int bytes;
	struct mq_attr attribute;
	attribute.mq_maxmsg = 5;
	attribute.mq_msgsize = MAX_MESSAGE;	
	int mq = (int )mq_open(_pipe_name.c_str(), mode, 0644, &attribute);
	if (mq < 0){
		EXITONERROR("Error");
	}
	return mq;
}

int MQRequestChannel::cread(void* msgbuf, int bufcapacity){
	return mq_receive(rfd, (char *)msgbuf, 16384, nullptr); 
}

int MQRequestChannel::cwrite(void* msgbuf, int len){
	return mq_send (wfd, (char *)msgbuf, len, 0);
}
