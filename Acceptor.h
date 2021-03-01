#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> //for bzero
#include <iostream>
#include <unistd.h>
#include <map>

using namespace std;


#include "Declear.h"
#include "IChannelCallBack.h"




class Acceptor : public IChannelCallBack{

public:
	 Acceptor(int epollfd);
	 ~Acceptor();
	 void setCallBack(IAcceptorCallBack* _pCallBack);
	 void start();
	 void virtual OnIn(int socket);
	 

private:
	int _epollfd;
	int _listenfd;
	int createAndListen();
	
	IAcceptorCallBack* _pCallBack;
	Channel* _pAcceptChannel;

};


#endif
