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


#include "IChannelCallBack.h"



#define MAX_EVENTS    10
#define MAX_LINE	  10
#define MAX_LISTENFD  10

using namespace std;


class TcpServer : public IChannelCallBack{
	public:
		TcpServer();
		~TcpServer();
		void start();
		virtual void OnIn(int sockfd);

	private:
		int _epollfd;
        int _listenfd;
		int createAndListen();
		
		struct epoll_event _events[MAX_EVENTS];
        map<int, Channel*> _channels;
};
