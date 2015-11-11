#include "wrappers.h"   // for socket wrapper fns
#include <stdio.h>      // for printf() and fprintf()

using namespace std;

int main(int argc, char *argv[]){

	int Sockfd;                      // Socket descriptor
	sockaddr_in EchoServAddr;        // Echo server address
	char *ServHost;                  // Host Name
	hostent *HostPtr;                // Host address struct
	int EchoServPort;                // Echo server port
	char EchoBuffer[MAX_SIZE];       // Buffer for echo string
	int EchoStringLen;               // Length of string to echo

	if(argc != 5){       // Test for correct number of arguments
		fprintf(stderr, "Usage: %s Host EchoWord Port\n",argv[0]);
		exit(1);
	} else {
		HostPtr = Gethostbyname(argv[1]);
        EchoServPort =atoi(argv[2]);
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
    cp->numThreads = atoi(argv[4]);

	// Send the pkt to the server
	send(Sockfd, (char*)cp, sizeof(ClientPkt), 0);

    // =================================== //
	// Listen for return message and print //




    // =================================== //

	printf("\n");    // Print a final linefeed
	close(Sockfd);
	exit(0);
}
