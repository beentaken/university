#include "wrappers.h"   // socket wrapper fns
#include <sys/wait.h>   // for waitpid()

#define RCVBUFSIZE 32   // Size of receive buffer

void HandleTCPClient(int ClntSocket);
extern "C" void SigChldHandler( int Signum );

void InitGame(GamePktType& GamePkt);
bool GameWon(char Game[3][3], char TstCh);
void GetCompMove(char Game[3][3]);

int main(int argc, char *argv[])
{
    int ServSock;                    // Socket descriptor for server
    int ClntSock;                    // Socket descriptor for client
    unsigned short EchoServPort;     // Server port
    sockaddr_in EchoServAddr;        // Local address
    sockaddr_in EchoClntAddr;        // Client address
    pid_t ProcessID;                 // Process ID from fork()
    unsigned int ChildProcCount = 0; // Number of child processes

    if (argc != 2){     // Test for correct number of arguments
        fprintf(stderr, "Usage:  %s Port\n", argv[0]);
        exit(1);
    }

    EchoServPort = atoi(argv[1]);  // First arg:  local port

    // Create socket for incoming connections
    ServSock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Construct local address structure
    memset((char*)&EchoServAddr, 0, sizeof(EchoServAddr));   /* Zero out structure */
    EchoServAddr.sin_family = AF_INET;                /* Internet address family */
    EchoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    EchoServAddr.sin_port = htons(EchoServPort);              /* Local port */

    // Bind to the local address
    Bind(ServSock, (sockaddr*)&EchoServAddr, sizeof(EchoServAddr));

    // Mark the socket so it will listen for incoming connections
    Listen(ServSock, 5);

    signal(SIGCHLD, SigChldHandler); // for preventing zombies

    for(;;){ // Run forever

        // Set the size of the in-out parameter
        int ClntLen = sizeof(EchoClntAddr);

        // Wait for a client to connect
        ClntSock = Accept(ServSock, (sockaddr*) &EchoClntAddr,&ClntLen);
	if (ClntSock == -1) continue;

        // ClntSock is connected to a client!
        printf("Handling client %s\n", inet_ntoa(EchoClntAddr.sin_addr));

        // Fork child process and report any errors
        if ((ProcessID = fork()) < 0){
            perror("fork() failed");
            exit(1);
        }
        if (ProcessID == 0){   // If this is the child process
            close(ServSock);   // Child closes (deallocates) its parent socket descriptor
            HandleTCPClient(ClntSock);
            exit(1);           // Child process terminates
        }
        printf("With child process: %d\n", (int)ProcessID);
        close(ClntSock);       // Parent closes (deallocates) its child socket descriptor
        ChildProcCount++;      // Increment number of outstanding child processes
    }
    // NOT REACHED
}

void HandleTCPClient(int ClntSocket){
	char GameBegin[RCVBUFSIZE];
	int RecvMsgSize;                    // Size of received message
	GamePktType GamePkt;
	srand(time(NULL));

	// Receive message from client
	if(recv(ClntSocket, GameBegin, RCVBUFSIZE, 0) < 0) {
		perror("recv() failed"); exit(1);
	}

	InitGame(GamePkt);

	GamePkt.Status = ePlayGame;

	for (int GameNo = 0; GameNo < 5; GameNo++) {

		if (GamePkt.Status != ePlayGame) {
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++) {
                    GamePkt.Game[i][j] = ' ';
				}
			}

			GamePkt.MoveNo = 0;
			GamePkt.Status = ePlayGame;
		}

		for (;;) {
            // send first message to client sending game data and requesting move
			if (GamePkt.MoveNo != 0 || (strcmp(GameBegin, "-u") == 0 && GameNo % 2 == 0)) {
				if(send(ClntSocket, (char*)&GamePkt, sizeof(GamePktType), 0) != sizeof(GamePktType)){
					perror("send() failed"); exit(1);
				}

				if (recv(ClntSocket, (char*)&GamePkt, sizeof(GamePktType), 0) <= 0){
					perror("recv() failed or connection closed prematurely!");
					exit(1);
				}
			}

            // check if user won
			if (GameWon(GamePkt.Game, cUser)) {
			    GamePkt.Status = eUserWin;
                GamePkt.UserScore++;
                // break
                break;
            }
			else if (GamePkt.MoveNo == 9) {
                GamePkt.Status = eDraw;
                // break
                break;
            }

            // user ms.d
			GetCompMove(GamePkt.Game);
			GamePkt.MoveNo++;

			if (GameWon(GamePkt.Game, cComputer)) {
                GamePkt.Status = eCompWin;
                GamePkt.CompScore++;
                // break
                break;
            }
			else if (GamePkt.MoveNo == 9) {
                GamePkt.Status = eDraw;
                // break
                break;
            }
		}

		if(send(ClntSocket, (char*)&GamePkt, sizeof(GamePktType), 0) != sizeof(GamePktType)){
			perror("send() failed"); exit(1);
		}
	}

	if(send(ClntSocket, (char*)&GamePkt, sizeof(GamePktType), 0) != sizeof(GamePktType)){
		perror("send() failed"); exit(1);
	}

	close(ClntSocket);    /* Close client socket */
}


extern "C" void SigChldHandler( int Signum ){
// Catch SIGCHLD signals so child processes don't become zombies.
	pid_t pid;
	int stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0 );
	return;
}

void InitGame(GamePktType& GamePkt)
{
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++) {
            GamePkt.Game[i][j] = ' ';
		}
    }
	GamePkt.UserScore = 0;
	GamePkt.CompScore = 0;
	GamePkt.MoveNo = 0;
	GamePkt.Status = eInitGame;
}

void GetCompMove(char Game[3][3])
{
    // Check for best moves horizontal
	for (int i = 0; i < 3; i++) {
        int rUser = 0, rComputer = 0;
        // find more than 2 recurring characters in single line
        for (int j = 0; j < 3; j++){
            if (Game[i][j] == cComputer)
                rUser++;
            if (Game[i][j] == cUser)
                rComputer++;
        }

        // if more than 2 recurring characters, block win or go for win
        if (rUser == 2 || rComputer == 2) {
            for (int j = 0; j < 3; j++){
                if (Game[i][j] == ' ') {
                    Game[i][j] = cComputer;
                    return;
                }
            }
        }
	}

    // check for best moves vertical
	for (int i = 0; i < 3; i++) {
        int rUser = 0, rComputer = 0;
        for (int j = 0; j < 3; j++){
            if (Game[j][i] == cComputer)
                rUser++;
            if (Game[j][i] == cUser)
                rComputer++;
        }

        if (rUser == 2 || rComputer == 2) {
            for (int j = 0; j < 3; j++){
                if (Game[j][i] == ' ') {
                    Game[j][i] = cComputer;
                    return;
                }
            }
        }
	}


    // diagonal win
    if (Game[0][0] == cComputer && Game[1][1] == cComputer && Game[2][2] == ' ') {
        Game[2][2] = cComputer;
        return;
    }
    else if (Game[0][0] == cComputer && Game[2][2] == cComputer && Game[1][1] == ' ') {
        Game[1][1] = cComputer;
        return;
    }
    else if (Game[1][1] == cComputer && Game[2][2] == cComputer && Game[0][0] == ' ') {
        Game[0][0] = cComputer;
        return;
    }
    // alt diagonal win
    else if (Game[0][2] == cComputer && Game[1][1] == cComputer && Game[2][0] == ' ') {
        Game[2][0] = cComputer;
        return;
    }
    else if (Game[0][2] == cComputer && Game[2][0] == cComputer && Game[1][1] == ' ') {
        Game[1][1] = cComputer;
        return;
    }
    else if (Game[1][1] == cComputer && Game[2][0] == cComputer && Game[0][2] == ' ') {
        Game[0][2] = cComputer;
        return;
    }

    // diagonal block
    if (Game[0][0] == cUser && Game[1][1] == cUser && Game[2][2] == ' ') {
        Game[2][2] = cComputer;
        return;
    }
    else if (Game[0][0] == cUser && Game[2][2] == cUser && Game[1][1] == ' ') {
        Game[1][1] = cComputer;
        return;
    }
    else if (Game[1][1] == cUser && Game[2][2] == cUser && Game[0][0] == ' ') {
        Game[0][0] = cComputer;
        return;
    }
    // alt diagonal block
    else if (Game[0][2] == cUser && Game[1][1] == cUser && Game[2][0] == ' ') {
        Game[2][0] = cComputer;
        return;
    }
    else if (Game[0][2] == cUser && Game[2][0] == cUser && Game[1][1] == ' ') {
        Game[1][1] = cComputer;
        return;
    }
    else if (Game[1][1] == cUser && Game[2][0] == cUser && Game[0][2] == ' ') {
        Game[0][2] = cComputer;
        return;
    }

    int i, j;
    do {
        i = rand() % 3;
        j = rand() % 3;
    } while(Game[i][j] != ' ');
    Game[i][j] = cComputer;
}

bool GameWon(char Game[3][3], char TstCh)
{
    // returns true if row of TstCh found
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++) {
            if (Game[i][j] != TstCh)
                break;
            else if (j == 2)
                return true;
        }
    }

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++) {
            if (Game[j][i] != TstCh)
                break;
            else if (j == 2)
                return true;
        }
    }

    if (Game[0][0] == TstCh && Game[1][1] == TstCh && Game[2][2] == TstCh)
        return true;

    if (Game[2][0] == TstCh && Game[1][1] == TstCh && Game[0][2] == TstCh)
        return true;

	return false;
}
