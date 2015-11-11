/*
  wrappers.h - Wrapper fns prototypes for CSCI212 socket programs
  Most programs using ports and sockets need to #include the same
  cluster of header files, wrapper functions etc.
  These #includes & wrapper functions are for UNIX sockets.
  Ver1: Koren Ward
*/

#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

#include <unistd.h>     // unix system fns
#include <stdlib.h>     // things like exit()
#include <sys/types.h> 	// a variety of typedefs
#include <sys/socket.h>	// socket stuff
#include <netinet/in.h>	// functions for converting
#include <netdb.h>      // dotted decimal strings
#include <arpa/inet.h> 	// to IP numbers
#include <signal.h>		  // signal handling
#include <sys/signal.h> // signal handling
#include <errno.h>		  // error reports
#include <iostream.h>	  // stream stuff...
#include <ctype.h>		  // char stuff...
#include <string.h>     // string stuff...
#include <dirent.h>     // dir stuff...
#include <stdio.h>      // for perror()
#include <map>


const int SERV_TCP_PORT = 40912; // server's default port number
const int MAX_SIZE = 80;        // send/receive buffer size
const char cUser = 'x';
const char cComputer = 'o';

enum StatusType{eInitGame,ePlayGame,eUserWin,eCompWin,eDraw};

struct ClientPkt{
    char fileName[100];
    int numThreads;
};

struct ServerData {
    std::map<std::string, int> commonWords;
    std::map<char, int> characters;
    int charNo, wordNo, lineNo;
    double totalWordLength;
    std::string longestWord;

    ServerData(){
        charNo = 0;
        wordNo = 0;
        lineNo = 0;
        totalWordLength = 0.0;
    }

    double AverageWordLength() {
        return totalWordLength / wordNo;
    }

    std::string MostFrequentWord() {
        std::string commonword = "";
        for( std::map<std::string,int>::iterator ii = commonWords.begin(); ii != commonWords.end(); ++ii)
        {
            if (ii->second > commonWords[commonword])
                commonword = ii->first;
        }
        return commonword;
    }

    void TopCharacters(char chars[], int frequency[], int amount) {
        for (int i = 0; i < amount; i++) {
            char c = ' ';
            for( std::map<char,int>::iterator ii = characters.begin(); ii != characters.end(); ++ii)
            {
                if (ii->second > characters[c])
                    c = ii->first;
            }
            chars[i] = c;
            frequency[i] = characters[c];

            characters[c] = 0;
        }
    }
};

struct ServerPkt {
    char commonWord[100];
    char charVal[20];
    int charNum[20];
    int charNo, wordNo, lineNo;
    double aveWordLength;
    char longestWord[100];
};

//============= Wrapper Functions ========================


int Socket( int Domain, int Type, int Protocol );
void Bind( int Sockfd, sockaddr *MyAddr, int AddrLen );
void Listen( int Sockfd, int Backlog );
void Connect(int Sockfd, sockaddr *ServAddr, int Size);
int Accept( int Sockfd, sockaddr *Addr, int *AddrLen );
hostent *Gethostbyname(char *ServHost);


#endif

