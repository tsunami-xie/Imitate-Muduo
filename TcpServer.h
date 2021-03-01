#ifndef TCPSERVER_H
#define TCPSERVER_H


#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> //for bzero
#include <iostream>
#include <unistd.h>
#include <map>

#include "Declear.h"
#include "IAcceptorCallBack.h"


#define MAX_EVENTS    10
#define MAX_LINE	  10
#define MAX_LISTENFD  10

using namespace std;


class TcpServer: public IAcceptorCallBack{

	public:
		TcpServer();
		~TcpServer();
		void start();
		//virtual void OnIn(int sockfd);

	private:
		
		int _epollfd;
		/*
        int _listenfd;
		int createAndListen();
		*/
		
		struct epoll_event _events[MAX_EVENTS];
        map<int, Channel*> _channels;
		Acceptor* _pAcceptor;	
};

#endif
