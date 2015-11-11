//  wrappers.cpp - Wrapper functions for CSCI212 socket programs

#include "wrappers.h"


//============= Wrapper Functions ========================

int Socket( int Domain, int Type, int Protocol )
{
	int Sockfd;
	if ((Sockfd = socket(Domain, Type, Protocol)) < 0)
	{
		perror( "socket creation" );
		exit(1);
	}
	return Sockfd;
}

void Bind( int Sockfd, sockaddr *MyAddr, int AddrLen )
{
	if (bind( Sockfd, MyAddr, AddrLen ) < 0)
	{
		perror( "socket bind" );
		exit(1);
	}
}

void Listen( int Sockfd, int Backlog )
{
	if (listen(Sockfd, Backlog ) < 0)
	{
		perror( "socket listen" );
		exit(1);
	}
}

void Connect(int Sockfd, sockaddr *ServAddr, int Size)
{
	if(connect(Sockfd, ServAddr, Size) < 0)
	{
		perror( "Can't connect to server!");
		exit(1);
	}
}

int Accept( int Sockfd, sockaddr *Addr, int *AddrLen )
{
	int Connfd;
	if ((Connfd = accept(Sockfd, Addr, AddrLen)) < 0)
	{
		if ((errno != EPROTO) && (errno != ECONNABORTED) && (errno != EINTR))
			perror( "socket accept" );
	}
	return Connfd;
}

struct hostent *Gethostbyname(char *ServHost)
{
	hostent *HostPtr;
	if((HostPtr = gethostbyname(ServHost)) == NULL)
	{
		perror( "gethostbyname error");
		exit(1);
	}
	return HostPtr;
}
