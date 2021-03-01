#include "Acceptor.h"
#include "Channel.h"
#include "Define.h"

Acceptor::Acceptor(int epollfd)
    :_epollfd(epollfd)
    ,_listenfd(-1)
    ,_pAcceptChannel(NULL)
    ,_pCallBack(NULL)
{}

void Acceptor::setCallBack(IAcceptorCallBack* callBack){
    _pCallBack = callBack;
}

int Acceptor::createAndListen(){
	int on =1;
	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in servaddr;
	fcntl(listenfd, F_SETFL, O_NONBLOCK);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(11111);
	if(-1==bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))){
		 cout << "bind error, errno:" << errno << endl; 
	}
	if(-1==listen(listenfd,MAX_LISTENFD)){
		cout << "listen error, errno:" << errno << endl; 
	}
	return listenfd;

}


void Acceptor::OnIn(int sockfd){

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

		/*add tcpconnection */
	
	 }
}


void Acceptor::start(){
    _listenfd = createAndListen();
    // Memory Leak !!!
    _pAcceptChannel = new Channel(_epollfd, _listenfd); 
    _pAcceptChannel->setCallBack(this);
    _pAcceptChannel->enableReading();
}
