#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> //for bzero
#include <iostream>
#include <unistd.h>


#define MAX_EVENTS  10
#define MAX_LINE	10


using namespace std;


int createAndListen(){
	int on =0;
	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in servaddr;
	fcntl(listenfd, F_SETFL, O_NONBLOCK);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(11111);
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	listen(listenfd,10);
	return listenfd;

}



int main(int args, char** argv){

	cout<<"------main------------"<<endl;
	struct epoll_event ev,events[MAX_EVENTS];
	int listenfd,connfd,sockfd;
	int readlength;
	char line[MAX_LINE];
	struct sockaddr_in cliaddr;
	socklen_t clilen=sizeof(struct  sockaddr_in);
	int epollfd=epoll_create(1);
	if(epollfd<0){
		cout << "epoll_create error, error:" << epollfd << endl;
	}
	listenfd = createAndListen();
	cout<<"listenfd:"<<listenfd<<endl;
	ev.data.fd = listenfd;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
	 for(;;){
        int fds = epoll_wait(epollfd, events, MAX_EVENTS, -1);

		for(int i=0;i<fds;i++){		
			if(events[i].data.fd == listenfd){
				connfd = accept(listenfd, (sockaddr*)&cliaddr, (socklen_t*)&clilen);
				 if(connfd > 0)
                {
                    cout << "new connection from " 
                         << "[" << inet_ntoa(cliaddr.sin_addr) 
                         << ":" << ntohs(cliaddr.sin_port) << "]" 
                         << " accept socket fd:" << connfd 
                         << endl;
                }
                else
                {
                    cout << "accept error, connfd:" << connfd 
                         << " errno:" << errno << endl; 
                }
				fcntl(connfd,F_SETFL,O_NONBLOCK);
				ev.data.fd=connfd;
				ev.events=EPOLLIN;
				epoll_ctl(epollfd,EPOLL_CTL_ADD,connfd,&ev);		
			
			}
			else if(events[i].events & EPOLLIN){
				sockfd = events[i].data.fd;
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

	

	 }
	
}
