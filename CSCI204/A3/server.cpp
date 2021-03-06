#include "wrappers.h"   // socket wrapper fns
#include <sys/wait.h>   // for waitpid()
#include <fstream>
#include <map>
#include <sstream>

#define RCVBUFSIZE 32   // Size of receive buffer

extern "C" {
    #include <pthread.h>
    #include <unistd.h>
}

ClientPkt currentClientPkt;
ServerPkt currentServerPkt;

pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;

void HandleTCPClient(int ClntSocket);
extern "C" void SigChldHandler( int Signum );

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

extern "C" void * thread_process(void * thread_nr)
{
    int n = *(static_cast<int*>(thread_nr));

    std::fstream fin(currentClientPkt.fileName, std::ios::binary | std::ios::in);
    int lines = (currentServerPkt.lineNo / currentClientPkt.numThreads) + 1;

    std::map<std::string, int> commonWord;
    std::map<char, int> topCharacters;
    int words = 0, chars = 0, totalWordLength = 0;
    std::string longestWord;

    // figure out how many words
    for (int i = 0; !fin.eof(); i++) {
        // get string
        std::string str;
        getline(fin, str);

        if (i >= n * lines && i < (n+1) * lines) {
            // convert to lowercase and remove punctuation
            for (int i = 0; i < str.length(); i++)
            {
                str[i] = tolower(str[i]);
                if (!isalpha(str[i]) && (str[i] != '\'' || str[i] != '-'))
                    str[i] = ' ';
            }

            // check for words
            std::stringstream ss(str);
            std::string s;
            while (ss >> s) {
                commonWord[s]++;
                words++;

                totalWordLength += s.length();

                if (s.length() > longestWord.length())
                    longestWord = s;
            }
        }
    }

    // find out character frequence
    fin.seekg(0);
    char c;
    do {
        c = fin.get();
        topCharacters[c]++;
        chars++;
    } while (!fin.eof());

/*
    for (int i = 0; i < bfrlen; i++){

        currentServerPkt->
        count[buffer[i]]++;

        std::cout << buffer[i];
    }
*/
    // update server pkt
    // lock serverpkt from editing by other threads
    pthread_mutex_lock(&server_mutex);

    // add all the words
    for( std::map<std::string,int>::iterator ii = commonWord.begin(); ii != commonWord.end(); ++ii)
    {
        currentServerPkt.commonWord[ii.first()] += ii.second();
    }

    // add all the characters
    for( std::map<char,int>::iterator ii = topCharacters.begin(); ii != topCharacters.end(); ++ii)
    {
        currentServerPkt.characters[ii.first()] += ii.second();
    }

    // compare longest word
    if (currentServerPkt.longestWord.length() < longestWord.length())
        currentServerPkt.longestWord = longestWord;

    // add word total length
    currentServerPkt.totalWordLength += totalWordLength;

    // add other stats
    currentServerPkt.charNo += chars;
    currentServerPkt.wordNo += words;

    // unlock serverpkt
    pthread_mutex_unlock(&server_mutex);

    cout << "Thread " << n <<" has finished"  << endl;

    pthread_exit(NULL);
    return 0;
}

void HandleTCPClient(int ClntSocket){
    ClientPkt *cp = new ClientPkt();
	int RecvMsgSize;            // Size of received message

    // receive filename and number of threads
    if(recv(ClntSocket, (char*)cp, sizeof(ClientPkt), 0) < 0) {
        perror("recv() failed"); exit(1);
    }

    currentClientPkt = *(cp);

    // loop through file and find out number of lines
    std::fstream fin(currentClientPkt.fileName, std::ios::binary | std::ios::in);


    // get number of lines
    std::string str;
    do {
        getline(fin, str);
        currentServerPkt.lineNo++;
    } while (!fin.eof());

    // process file
    pthread_t *thread = new pthread_t[cp->numThreads];

    cout << "starting all threads..." << endl;

    int *temp_arg = new int[cp->numThreads];
    for(int i = 0; i < cp->numThreads; i++){
        temp_arg[i] = i;
        int result = pthread_create(&thread[i], NULL, thread_process, static_cast<void*>(&(temp_arg[i])));

        if (result !=0)
           cout << "Error creating thread " << i << ". Return code:" << result <<  endl;
    }

    for(int i = 0; i < cp->numThreads; i++)
    {
        pthread_join(thread[i], NULL);
    }

    cout << "All threads completed." << endl ;

    cout << "Filename: " << currentClientPkt.fileName;
    cout << "No. characters: " << currentServerPkt.charNo;
    cout << "No. words: " << currentServerPkt.wordNo;
    cout << "No. lines: " << currentServerPkt.lineNo;
    cout << "Ave. word length: " << currentServerPkt.AverageWordLength();
    cout << "Largest word: " << currentServerPkt.longestWord;



    // Send client pkt containing summary data
    /*if(send(ClntSocket, (char*)&GamePkt\, sizeof(GamePktType), 0) != sizeof(GamePktType)){
        perror("send() failed"); exit(1);
    }*/
    // =================================== //
	close(ClntSocket);    /* Close client socket */
}


extern "C" void SigChldHandler( int Signum ){
// Catch SIGCHLD signals so child processes don't become zombies.
	pid_t pid;
	int stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0 );
	return;
}
