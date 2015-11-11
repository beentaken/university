#include "wrappers.h"   // for socket wrapper fns
#include <stdio.h>      // for printf() and fprintf()

void DrawGame(char Game[3][3]);
void GetUserMove(char Game[3][3]);

int main(int argc, char *argv[]){

	int Sockfd;                      // Socket descriptor
	sockaddr_in EchoServAddr;        // Echo server address
	char *ServHost;                  // Host Name
	hostent *HostPtr;                // Host address struct
	int EchoServPort;                // Echo server port
	char EchoBuffer[MAX_SIZE];       // Buffer for echo string
	int EchoStringLen;               // Length of string to echo
	GamePktType GamePkt;		 	 // Game package
	int GamePktSize;				 // Game package size

	if(argc != 2 && argc != 4){       // Test for correct number of arguments
		fprintf(stderr, "Usage: %s Host EchoWord Port\n",argv[0]);
		exit(1);
	}

	if(argc == 4) {
		HostPtr = Gethostbyname(argv[1]);
		EchoServPort = atoi(argv[2]); /* Use given port, if any */
	}
	else {
		char HostName[10] = "localhost";
		HostPtr = Gethostbyname(HostName);
		EchoServPort = 40912;  // 7 is the well-known port for the echo service
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

	// Send the string to the server
	if (argc == 4) send(Sockfd, argv[3], 3, 0);
	else send(Sockfd, argv[1], 3, 0);

	for (int GameNo = 0; GameNo < 5; GameNo++) {
		printf("\nG a m e  %d\n", GameNo+1);

		for(;;) {
            // check if response was recieved
			if (recv(Sockfd, (char*)&GamePkt, sizeof(GamePktType), 0) <= 0){
				perror("recv() failed");
				exit(1);
			}

			DrawGame(GamePkt.Game);

			if (GamePkt.Status == ePlayGame) {

				GetUserMove(GamePkt.Game);

				GamePkt.MoveNo++;

				DrawGame(GamePkt.Game);
				if(send(Sockfd, (char*)&GamePkt, sizeof(GamePktType), 0) != sizeof(GamePktType)){
					perror("send() failed"); exit(1);
				}

			}
			else {
				printf("\nUser's Score: %d\n", GamePkt.UserScore);
				printf("Computer's Score: %d\n", GamePkt.CompScore);
				break;
			}
		}
	}

	if(GamePkt.UserScore > GamePkt.CompScore) printf("   Y o u  W i n !");
	else if(GamePkt.UserScore < GamePkt.CompScore) printf("   C o m p u t e r  W i n s !");
	else printf("   I t s  A  D r a w !");
	printf("\n   G a m e  O v e r !");
	printf("\n   U s e r    %d  |  C o m p     %d", GamePkt.UserScore, GamePkt.CompScore);

	printf("\n");    // Print a final linefeed
	close(Sockfd);
	exit(0);
}

void DrawGame(char Game[3][3])
{
    cout << "\t1   2   3" << endl << endl;
    cout << "\t  |   |" << endl;
    cout << "a\t" << Game[0][0] << " | " << Game[0][1] << " | " << Game[0][2] << endl;
    cout << "      ----|---|----" << endl;
    cout << "b\t" << Game[1][0] << " | " << Game[1][1] << " | " << Game[1][2] << endl;
    cout << "      ----|---|----" << endl;
    cout << "c\t" << Game[2][0] << " | " << Game[2][1] << " | " << Game[2][2] << endl;
    cout << "\t  |   |" << endl;
}

void GetUserMove(char Game[3][3])
{
    char alpha = ' ', numeric = ' ';
    int i = 0, j = 0;
    do {
        do {
            if (alpha != ' ')
                cout << "Invalid Move. Try again => ";
            else
                cout << "Enter your move => ";
            cin >> alpha;
            cin >> numeric;
            cin.ignore(100, '\n');
        } while ((alpha < 'a' || alpha > 'c') && (numeric < '1' || numeric > '3'));

        i = alpha - 97;
        j = numeric - 49;
    } while (Game[i][j] != ' ');

    Game[i][j] = cUser;
}
