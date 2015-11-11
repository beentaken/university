#include "wrappers.h"   // for socket wrapper fns
#include <stdio.h>      // for printf() and fprintf()
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]){

	int Sockfd;                      // Socket descriptor
	sockaddr_in EchoServAddr;        // Echo server address
	char *ServHost;                  // Host Name
	hostent *HostPtr;                // Host address struct
	int EchoServPort;                // Echo server port
	char EchoBuffer[MAX_SIZE];       // Buffer for echo string
	int EchoStringLen;               // Length of string to echo

	if(argc != 5 && argc != 4){       // Test for correct number of arguments
		fprintf(stderr, "Usage: %s Host EchoWord Port\n",argv[0]);
		exit(1);
	} else {
		HostPtr = Gethostbyname(argv[1]);
        EchoServPort = atoi(argv[2]);
	}

	// Create a reliable, stream socket using TCP
	Sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Construct the server address structure
	memset(&EchoServAddr, 0, sizeof(EchoServAddr)); // Zero out structure
	EchoServAddr.sin_family = AF_INET;              // Internet address family
	EchoServAddr.sin_addr.s_addr = ((in_addr*)HostPtr->h_addr_list[0])->s_addr;
	EchoServAddr.sin_port  = htons(EchoServPort);   // Server port

	// Establish the connection to the echo server
	Connect(Sockfd, (sockaddr*)&EchoServAddr, sizeof(EchoServAddr));

	// Send filename and number of threads
	ClientPkt *cp = new ClientPkt();
	strcpy(cp->fileName, argv[3]);
	if (argc == 5)
        cp->numThreads = atoi(argv[4]);
    else
        cp->numThreads = 6;

	// Send the pkt to the server
	send(Sockfd, (char*)cp, sizeof(ClientPkt), 0);

    // =================================== //
	// Listen for return message and print //
	ServerPkt serverPkt;
    if (recv(Sockfd, (char*)&serverPkt, sizeof(ServerPkt), 0) <= 0){
        perror("recv() failed");
        exit(1);
    }

    // Print
    printf("%-*s %*s \n", 25, "Filename:", 20, cp->fileName);
    printf("%-*s %*d \n", 25, "No. characters:", 20, serverPkt.charNo);
    printf("%-*s %*d \n", 25, "No. words:", 20, serverPkt.wordNo);
    printf("%-*s %*d \n", 25, "No. words:", 20, serverPkt.lineNo);
    printf("%-*s %*f \n", 25, "Ave. word length:", 20, serverPkt.aveWordLength);
    printf("%-*s %*s \n", 25, "Largest word:", 20, serverPkt.longestWord);
    printf("%-*s %*s \n", 30, "Most frequently used word:", 15, serverPkt.commonWord);
    printf("20 most frequently used characters:\n");

    for (int i = 0; i < 20; i++){
        /*if (isalpha(serverPkt.charVal[i]))
            cout << serverPkt.charVal[i] << ": " << serverPkt.charNum[i] << endl;
        else if (serverPkt.charVal[i] == '\n')
            cout << "\'\\n\': " << serverPkt.charNum[i] << endl;
        /*/if (isalpha(serverPkt.charVal[i]))
            printf("%*-c: %*d \n", 4, serverPkt.charVal[i], 5, serverPkt.charNum[i]);
        else if (serverPkt.charVal[i] == '\n')
            printf("\'\\%-s\': %*d \n", "n", 5, serverPkt.charNum[i]);
        else
            printf("\'%-c\' : %*d \n", serverPkt.charVal[i], 5, serverPkt.charNum[i]);
    }



    /*
    cout << setw(50) << left << "Filename: " << right << cp->fileName << endl;
    cout << setw(50) << left << "No. characters: " << right << serverPkt.characters << endl;*/

    // =================================== //

	printf("\n");    // Print a final linefeed
	close(Sockfd);
	exit(0);
}
