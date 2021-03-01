#include <vector>
#include "TcpServer.h"
#include "Channel.h"
#include "Acceptor.h"




TcpServer::TcpServer(){

}

TcpServer::~TcpServer(){

}

#if 0

void TcpServer::OnIn(int sockfd){

	int connfd;
	int readlength;
	char line[MAX_LINE];
	struct sockaddr_in cliaddr;
	socklen_t clilen=sizeof(struct  sockaddr_in);
	

	 if(sockfd == _listenfd){
	 	connfd = accept(_listenfd, (sockaddr*)&cliaddr, (socklen_t*)&clilen);
		if(connfd > 0){
            cout << "new connection from " 
                 << "[" << inet_ntoa(cliaddr.sin_addr) 
                 << ":" << ntohs(cliaddr.sin_port) << "]" 
                 << " accept socket fd:" << connfd 
                 << endl;
        }
        else{
            cout << "accept error, connfd:" << connfd 
                 << " errno:" << errno << endl; 
        }
		fcntl(connfd,F_SETFL,O_NONBLOCK);

		Channel* pChannel = new Channel(_epollfd, connfd);
		pChannel->setCallBack(this);
		pChannel->enableReading();

		/*add tcpconnection class*/

		
	 }else{
		bzero(line, MAX_LINE);
		readlength = read(sockfd, line, MAX_LINE);
		cout<<"receive:"<<line<<endl;
		if(readlength==0){
			cout<<"close:"<<sockfd<<endl;
			close(sockfd);
		}
		write(sockfd, line, readlength);
	 }
}
#endif

void TcpServer::start(){
	cout<<"------main------------"<<endl;
	_epollfd=epoll_create(1);
	if(_epollfd<0){
		cout << "epoll_create error, error:" << _epollfd << endl;
	}

	/*add Acceptor class*/
	_pAcceptor = new Acceptor(_epollfd); // Memory Leak !!!
    _pAcceptor->setCallBack(this);
    _pAcceptor->start();
	
	


	for(;;){
        int fds = epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
		vector<Channel*> channels;
		for(int i=0;i<fds;i++){	
			Channel* pChannel = static_cast<Channel*>(_events[i].data.ptr);
    		pChannel->setRevents(_events[i].events);	
			channels.push_back(pChannel);	
			
		}
		
		vector<Channel*>::iterator it;
		for(it = channels.begin(); it != channels.end(); ++it)
		{
			(*it)->handleEvent();
		}
	}
}


