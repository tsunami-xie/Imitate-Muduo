#ifndef CHANNEL_H
#define CHANNEL_H

#include "Declear.h"


class Channel{
	public:
		Channel(int epollfd, int sockfd);
        ~Channel();
		void enableReading();
		void setCallBack(IChannelCallBack* callBack);
		void setRevents(int revents);
		void handleEvent();

		int _epollfd;
        int _sockfd;
        int _events;
        int _revents;


	private:
		
		IChannelCallBack* _callBack;

		void update();
		
		
};

#endif

