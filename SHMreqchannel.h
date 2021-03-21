#ifndef _SHMreqchannel_H_
#define _SHMreqchannel_H_

#include "common.h"
#include "Reqchannel.h"
#include "semaphore.h"
#include <string>
#include <sys/mman.h>
#include <fcntl.h>

class SHMQ{
    private:
        char * segment;
        sem_t* sender;
        sem_t* receiver;
        string name;
        int length;
        int fd;
    public:
        SHMQ(string _name, int _length): name(_name), length (_length){
            fd = shm_open(name.c_str(), O_RDWR|O_CREAT, 0644);
            if(fd < 0){
                EXITONERROR("Could Not create/open shared memory");
            }
            ftruncate(fd, length);
            segment = (char *) mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            if(!segment){
                 EXITONERROR("Cannot Map");
            }
            receiver = sem_open((name + "_rd").c_str(), O_CREAT, 0644, 1);
            sender = sem_open((name + "_sd").c_str(), O_CREAT, 0644, 0);
        }

        int shm_send(void* msg, int length){
            sem_wait(receiver);
            memcpy(segment, msg, length);
            sem_post(sender);
            return length;
        }

        int shm_receive(void* msg, int length){
            sem_wait(sender);
            memcpy(msg, segment, length);
            sem_post(receiver);
            return length;
        }

        ~SHMQ(){
            munmap(segment, length);
            sem_close(sender);
            sem_close(receiver);
            sem_unlink((name + "_rd").c_str());
            sem_unlink((name + "_sd").c_str());
            shm_unlink(name.c_str());
        }
};

class SHMRequestChannel: public RequestChannel{
private:
    SHMQ* sharedmemqueue1;
    SHMQ* sharedmemqueue2;
    int length;

public:
	SHMRequestChannel(const string _name, const Side _side, int length);
	
	~SHMRequestChannel();


	int cread (void* msgbuf, int bufcapacity);

	int cwrite (void *msgbuf , int msglen);
	

};

#endif