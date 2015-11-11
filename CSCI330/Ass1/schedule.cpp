#include <iostream>
#include <sstream>
#include <list>
#include <ctime>

using namespace std;

int gNumProcesses;
int gMaxTimeInc;
int gMaxBurstTime;
int gTimeSlice;

char processMarker = '0';

// Process Struct
struct Process {
	int WaitingTime;
	int ArrivalTime;
	int BurstTime;
	int RunTime;
	
	char Marker;	
};

// CPU Struct
struct CPU {
	Process* Running;
	int RunTime;
	
	list<Process> JobQueue;
	list<Process> ReadyQueue;
	list<Process> EndQueue;
	
	int Time;
	bool Done;
	
	list<char> Gantt;
};

// Method Declarations
void GetNextProcess(int&, int&);

// Helpers
int ArgToInt(const char*);
int ValidateArgs(int, const char**);
void BufferJobQueue(list<Process> &, int);

// debug prints
/*
void PrintProcesses(char*, list<Process>);
void PrintGantt(char*, CPU, list<Process>);
*/

// final Print
void PrintAllGantt(CPU &, CPU &, CPU &, CPU &, CPU &, list<Process>);
void PrintGanttChars(list<char>);

// scheduling functions
void FCFS(CPU &, list<Process>);
void RR(CPU &, list<Process>);
void SJF(CPU &, list<Process>);
void SRTF(CPU &, list<Process>);
void RAND(CPU &, list<Process>);

void MoveArrivals(CPU &);

// Main
int main(int argc, const char* argv[]) 
{	
	/*int seed = 0;
	srand(seed);*/
	
	// validate arguments
	if (ValidateArgs(argc, argv) == 2)
		return 2;
	
	// gNumProcesses, gMaxTimeInc, gMaxBurstTime, gTimeSlice
	gNumProcesses = ArgToInt(argv[1]);
	gMaxTimeInc   = ArgToInt(argv[2]);
	gMaxBurstTime = ArgToInt(argv[3]);
	gTimeSlice    = ArgToInt(argv[4]);
	
	// Get Job Queue
	list<Process> jobs;
	BufferJobQueue(jobs, gNumProcesses);
	
	if (jobs.size() != gNumProcesses) {
		cout << "JobQueue size is not equal to gNumProcesses." << endl;
		return 2;
	}
	
	// CPU's
	CPU fcfs, rr, sjf, srtf, rnd;
	
	// First Come First Serve
	FCFS(fcfs, jobs);
		
	// Round Robin
	RR(rr, jobs);	
	
	// Shortest Job First
	SJF(sjf, jobs);	
	
	// Shortest Remaining Time First
	SRTF(srtf, jobs);	
	
	// Random
	RAND(rnd, jobs);	
		
	// Print schedules	
	PrintAllGantt(fcfs, rr, sjf, srtf, rnd, jobs);
}

void PrintAllGantt(CPU &fcfs, CPU &rr, CPU &sjf, CPU &srtf, CPU &rnd, list<Process> jobs) 
{	
	cout << "\t\tFCFS\t\tRR\t\tSJF\t\tSRTF\t\tRAND" << endl;
	cout << /*"\tAT" << */"\tBT\tWT\tTT" << "\tWT\tTT" << "\tWT\tTT" << "\tWT\tTT" << "\tWT\tTT" << endl;
	
	int wTotal[5] = { 0, 0, 0, 0, 0 };
	int tTotal[5] = { 0, 0, 0, 0, 0 };	
	int count = 0;
	
	CPU cpus[5] = { fcfs, rr, sjf, srtf, rnd };
	
	for (list<Process>::iterator uit = jobs.begin();
		uit != jobs.end();
		uit++) {
		
		cout << uit->Marker << ":"
			/*<< "\t" << uit->ArrivalTime*/
			<< "\t" << uit->BurstTime;
			
		for (int i = 0; i < 5; i++) {
			
			CPU cpu = cpus[i];
			
			for (list<Process>::iterator it = cpu.EndQueue.begin();
					it != cpu.EndQueue.end();
					it++) {
							
				if (it->Marker == uit->Marker && it->ArrivalTime == uit->ArrivalTime) {
							
					cout << "\t" << it->WaitingTime 
						 << "\t" << it->BurstTime + it->WaitingTime;
					
					wTotal[i] += it->WaitingTime;
					tTotal[i] += it->BurstTime + it->WaitingTime;
				}
			}
			
		}
		
		count++;
		cout << endl;
	}
	
	// Average
	cout << "Average:";
	for (int i = 0; i < 5; i++) {
		 cout << "\t" << wTotal[i] / count << "\t" << tTotal[i] / count;
	}
	cout << endl;
	
	// Gantt Chart	
	cout << "FCFS:\t";
	PrintGanttChars(fcfs.Gantt);
	cout << endl;
	
	cout << "RR:\t";
	PrintGanttChars(rr.Gantt);
	cout << endl;
	
	cout << "SJF:\t";
	PrintGanttChars(sjf.Gantt);
	cout << endl;
	
	cout << "SRTF:\t";
	PrintGanttChars(srtf.Gantt);
	cout << endl;
	
	cout << "RAND:\t";
	PrintGanttChars(rnd.Gantt);
	cout << endl;
}

void PrintGanttChars(list<char> gantt) 
{
	for (list<char>::iterator it = gantt.begin();
			it != gantt.end();
			it++) {
		cout << *it;
	}
}

void RAND(CPU &cpu, list<Process> queue) 
{
	// seed for laughs
	srand(time(NULL));
	
	// CPU
	cpu.Running = NULL;
	cpu.RunTime = 0;
	cpu.Time = 0;
	cpu.JobQueue = queue;
	
	MoveArrivals(cpu);
	list<Process>::iterator current = cpu.ReadyQueue.begin();
	
	// loop until Done
	while (!cpu.Done) {
				
		// check job queue for an arriving process
		MoveArrivals(cpu);
			
		// if cpu not empty
		if (cpu.Running != NULL) {
			// runtime >= BurstTime
			if (cpu.Running->RunTime >= cpu.Running->BurstTime) {				
				// move process to EndQueue				
				cpu.EndQueue.splice(cpu.EndQueue.end(), cpu.ReadyQueue, current);
				cpu.Running = NULL;
			}
		} 
		
		// Pick Next Job
		if (cpu.Running == NULL && !cpu.ReadyQueue.empty()) // if cpu is empty
		{					
			// get random
			int i = 0;
			int next = rand() % cpu.ReadyQueue.size();
			for (list<Process>::iterator it = cpu.ReadyQueue.begin();
					it != cpu.ReadyQueue.end();
					++it) {
				if (i == next)
					current = it;
				i++;
			}			
					
			// move next process from ReadyQueue to Running 
			cpu.Running = &(*(current));						
			// reset RunTime to 0
			cpu.RunTime = 0;
		} 
		
		// end of loop, all processes in ReadyQueue have waiting time incremented
		for (list<Process>::iterator it = cpu.ReadyQueue.begin();
				it != cpu.ReadyQueue.end();
				it++) {
			if (it != current)
				it->WaitingTime++;
		}
		
		// increment runtime & time
		cpu.RunTime++;
		cpu.Time++;
		
		if (cpu.Running != NULL)
			cpu.Running->RunTime++;
		
		// Done = JobQueue.empty ReadyQueue.empty CPU.Running.empty
		cpu.Done = (cpu.JobQueue.empty() && cpu.ReadyQueue.empty() && cpu.Running == NULL);
		
		if (!cpu.Done) {
			// Record job in Gantt chart
			char gantt = '-';
			if (cpu.Running != NULL)
				gantt = cpu.Running->Marker;
			cpu.Gantt.push_back(gantt);
		}
	}
}

void SRTF(CPU &cpu, list<Process> queue) 
{	
	// CPU
	cpu.Running = NULL;
	cpu.RunTime = 0;
	cpu.Time = 0;
	cpu.JobQueue = queue;
	
	MoveArrivals(cpu);
	list<Process>::iterator current = cpu.ReadyQueue.begin();
	
	// loop until Done
	while (!cpu.Done) {
				
		// check job queue for an arriving process
		int readyQueueSize = cpu.ReadyQueue.size();
		MoveArrivals(cpu);
		// if arrivals, pick next job
		if (cpu.ReadyQueue.size() > readyQueueSize) {
			cpu.Running = NULL;
		}
			
			
		// if cpu not empty
		if (cpu.Running != NULL) {
			// runtime >= BurstTime
			if (cpu.Running->RunTime >= cpu.Running->BurstTime) {								
				// move process to EndQueue				
				cpu.EndQueue.splice(cpu.EndQueue.end(), cpu.ReadyQueue, current);
				cpu.Running = NULL;
				
				current = cpu.ReadyQueue.begin();
			}
		} 
		
		
		// Pick Next Job
		if (cpu.Running == NULL && !cpu.ReadyQueue.empty()) // if cpu is empty
		{					
			// get next shortest time remaining
			current = cpu.ReadyQueue.begin();
			for (list<Process>::iterator it = cpu.ReadyQueue.begin();
					it != cpu.ReadyQueue.end();
					++it) {
				if (it->BurstTime - it->RunTime < current->BurstTime - current->RunTime)
					current = it;
			}			
					
			// move next process from ReadyQueue to Running 
			cpu.Running = &(*(current));						
			// reset RunTime to 0
			cpu.RunTime = 0;
		} 
		
		
		// end of loop, all processes in ReadyQueue have waiting time incremented
		for (list<Process>::iterator it = cpu.ReadyQueue.begin();
				it != cpu.ReadyQueue.end();
				it++) {
			if (it != current)
				it->WaitingTime++;
		}
		
		// increment runtime & time
		cpu.RunTime++;
		cpu.Time++;
		
		if (cpu.Running != NULL)
			cpu.Running->RunTime++;
		
		// Done = JobQueue.empty ReadyQueue.empty CPU.Running.empty
		// cout << cpu.JobQueue.size() << " | " << cpu.ReadyQueue.size() << endl;
		cpu.Done = (cpu.JobQueue.empty() && cpu.ReadyQueue.empty() && cpu.Running == NULL);
		
		if (!cpu.Done) {
			// Record job in Gantt chart
			char gantt = '-';
			if (cpu.Running != NULL)
				gantt = cpu.Running->Marker;
			cpu.Gantt.push_back(gantt);
		}
	}	
}

void SJF(CPU &cpu, list<Process> queue) 
{	
	// CPU
	cpu.Running = NULL;
	cpu.RunTime = 0;
	cpu.Time = 0;
	cpu.JobQueue = queue;
	
	MoveArrivals(cpu);
	list<Process>::iterator current = cpu.ReadyQueue.begin();
	
	// loop until Done
	while (!cpu.Done) {
				
		// check job queue for an arriving process
		MoveArrivals(cpu);
		
		// if cpu not empty
		if (cpu.Running != NULL) {
			// runtime >= BurstTime
			if (cpu.RunTime >= cpu.Running->BurstTime) {				
				// move process to EndQueue				
				cpu.EndQueue.splice(cpu.EndQueue.end(), cpu.ReadyQueue, current);
				cpu.Running = NULL;
				
				current = cpu.ReadyQueue.begin();
			}
		} 
		
		// Pick Next Job
		if (cpu.Running == NULL && !cpu.ReadyQueue.empty()) // if cpu is empty
		{					
			// get next shortest remaining
			for (list<Process>::iterator it = cpu.ReadyQueue.begin();
					it != cpu.ReadyQueue.end();
					++it) {
				if (it->BurstTime < current->BurstTime)
					current = it;
			}			
					
			// move next process from ReadyQueue to Running 
			cpu.Running = &(*(current));						
			// reset RunTime to 0
			cpu.RunTime = 0;
		} 
		
		// end of loop, all processes in ReadyQueue have waiting time incremented
		for (list<Process>::iterator it = cpu.ReadyQueue.begin();
				it != cpu.ReadyQueue.end();
				it++) {
			if (it != current)
				it->WaitingTime++;
		}
		
		// increment runtime & time
		cpu.RunTime++;
		cpu.Time++;
		
		// Done = JobQueue.empty ReadyQueue.empty CPU.Running.empty
		cpu.Done = (cpu.JobQueue.empty() && cpu.ReadyQueue.empty() && cpu.Running == NULL);
		
		if (!cpu.Done) {
			// Record job in Gantt chart
			char gantt = '-';
			if (cpu.Running != NULL)
				gantt = cpu.Running->Marker;
			cpu.Gantt.push_back(gantt);
		}
	}	
}

void RR(CPU &cpu, list<Process> queue) {	
	// CPU
	cpu.Running = NULL;
	cpu.RunTime = 0;
	cpu.Time = 0;
	cpu.JobQueue = queue;
	
	list<Process>::iterator current = cpu.ReadyQueue.begin();
	
	// loop until Done
	while (!cpu.Done) {
				
		// check job queue for an arriving process
		MoveArrivals(cpu);
		
		// if cpu not empty
		if (cpu.Running != NULL && current != cpu.ReadyQueue.end()) {
			// runtime >= BurstTime
			if (cpu.Running->RunTime >= cpu.Running->BurstTime) {
				// 
				list<Process>::iterator it = current;
				it--;
				
				// move process to EndQueue				
				cpu.EndQueue.splice(cpu.EndQueue.end(), cpu.ReadyQueue, current);
				cpu.Running = NULL;
				
				current = ++it;
			}
			// runtime > gTimeSlice, move to next job 
			else if (cpu.RunTime % gTimeSlice == 0) {
				cpu.Running = NULL;
				++current;
			}
		} 
		
		// Pick Next Job
		if (cpu.Running == NULL && !cpu.ReadyQueue.empty()) // if cpu is empty
		{			
			if (current == cpu.ReadyQueue.end())
				current = cpu.ReadyQueue.begin();
				
			// move next process from ReadyQueue to Running 
			cpu.Running = &(*(current));						
			// reset RunTime to 0
			cpu.RunTime = 0;
		} 
		
		// end of loop, all processes in ReadyQueue have waiting time incremented
		for (list<Process>::iterator it = cpu.ReadyQueue.begin();
				it != cpu.ReadyQueue.end();
				it++) {
			if (it != current)
				it->WaitingTime++;
		}
		
		// increment runtime & time
		cpu.RunTime++;
		cpu.Time++;
		
		// increment process runtime
		if (cpu.Running != NULL)
			cpu.Running->RunTime++;
		
		// Done = JobQueue.empty ReadyQueue.empty CPU.Running.empty
		cpu.Done = (cpu.JobQueue.empty() && cpu.ReadyQueue.empty() && cpu.Running == NULL);
		
		if (!cpu.Done) {
			// Record job in Gantt chart
			char gantt = '-';
			if (cpu.Running != NULL)
				gantt = cpu.Running->Marker;
			cpu.Gantt.push_back(gantt);
		}
	}		
}

void FCFS(CPU &cpu, list<Process> queue) {
	// CPU
	cpu.Running = NULL;
	cpu.RunTime = 0;
	cpu.Time = 0;
	cpu.JobQueue = queue;
	
	// loop until Done
	while (!cpu.Done) {
				
		// check job queue for an arriving process
		MoveArrivals(cpu);
		
		// if cpu not empty
		if (cpu.Running != NULL) {
			// runtime >= BurstTime
			if (cpu.RunTime >= cpu.Running->BurstTime) {
				// move process to EndQueue
				cpu.EndQueue.splice(cpu.EndQueue.end(), cpu.ReadyQueue, cpu.ReadyQueue.begin());
				cpu.Running = NULL;
			}
		} 
		
		if (cpu.Running == NULL && !cpu.ReadyQueue.empty()) // if cpu is empty
		{
			// move next process from ReadyQueue to Running 
			cpu.Running = &(*(cpu.ReadyQueue.begin()));						
			// reset RunTime to 0
			cpu.RunTime = 0;
		} 
		
		// end of loop, all processes in ReadyQueue have waiting time incremented
		for (list<Process>::iterator it = cpu.ReadyQueue.begin();
				it != cpu.ReadyQueue.end();
				it++) {
			if (it != cpu.ReadyQueue.begin())
				it->WaitingTime++;
		}
		
		// increment runtime & time
		cpu.RunTime++;
		cpu.Time++;
		
		// Done = JobQueue.empty ReadyQueue.empty CPU.Running.empty
		// cout << cpu.JobQueue.size() << "|" << cpu.ReadyQueue.size() << "|" << cpu.Running << endl;
		cpu.Done = (cpu.JobQueue.empty() && cpu.ReadyQueue.empty() && cpu.Running == NULL);
		
		if (!cpu.Done) {
			// Record job in Gantt chart
			char gantt = '-';
			if (cpu.Running != NULL)
				gantt = cpu.Running->Marker;
			cpu.Gantt.push_back(gantt);
		}
	}	
}

void MoveArrivals(CPU &cpu) {
	if (!cpu.JobQueue.empty()) {
		// move arriving process from job queue to read queue
		for (list<Process>::iterator it = cpu.JobQueue.begin();
				it != cpu.JobQueue.end();
				++it) {
			if (it->ArrivalTime <= cpu.Time) {
				cpu.ReadyQueue.splice(cpu.ReadyQueue.end(), cpu.JobQueue, it);
				it = cpu.JobQueue.begin();
			}
		}			
	}
}

void GetNextProcess(int &ArrivalTime, int &BurstTime)
{
    static int PrevTime = -1;
    if (PrevTime == -1) PrevTime = ArrivalTime = 0;
    else ArrivalTime = PrevTime + 1 + rand() % gMaxTimeInc;
    BurstTime = 1 + rand() % gMaxBurstTime;
    PrevTime = ArrivalTime;
}

void BufferJobQueue(list<Process> &queue, int numProcesses) 
{
	// clear job queue
	queue.clear();
	
	// add new jobs
	for (int i = 0; i < numProcesses; i++) 
	{
		Process process;
		process.RunTime = 0;
		process.WaitingTime = 0;
		
		// assign some random times to process
		GetNextProcess(process.ArrivalTime, process.BurstTime);
		
		// send identifier
		process.Marker = processMarker++;
		
		// if > 62 reset identifier
		if (processMarker > 'z')
			processMarker = '0';
		else if (processMarker > 'Z' && processMarker < 'a')
			processMarker = 'a';
		else if (processMarker > '9' && processMarker < 'A')
			processMarker = 'A';
		
		// add to job queue
		queue.push_back(process);
	}
}

// validate all arguments except for the first one
// must be int, 0 < value < 101
int ValidateArgs(int argc, const char* argv[]) 
{
	if (argc != 5) {
		cout << "Require 5 arguments." << endl;
		return 2;
	}
	
	for (int i = 1; i < argc; i++) {
		int arg = ArgToInt(argv[i]);
		if (arg < 1 || arg > 100) {
			cout << "Argument " << i << " is out of range. (1-100)." << endl;
			return 2;
		}
	}
}

// convert char array to integer
int ArgToInt(const char* num) {
	stringstream stream;
	stream << num;
	
	int i;
	stream >> i;
	
	return i;
}

/*
 * Debug Printing Functions
 *//*
void PrintProcesses(char* message, list<Process> queue) 
{
	cout << message << ": ";
	for (list<Process>::iterator it = queue.begin();
			it != queue.end();
			it++) {
		cout << it->Marker << " | ";
	}
	cout << endl;
}


void PrintGantt(char* name, CPU cpu, list<Process> jobs) {
	cout << name << endl;
	cout << "\tBT\tWT\tTT\tAT" << endl;
	
	int wTotal = 0;
	int tTotal = 0;	
	int count = 0;
	
	for (list<Process>::iterator uit = jobs.begin();
		uit != jobs.end();
		uit++) {
			
		for (list<Process>::iterator it = cpu.EndQueue.begin();
				it != cpu.EndQueue.end();
				it++) {
						
			if (it->Marker == uit->Marker) {
						
				cout << it->Marker << ":"
					<< "\t" << it->BurstTime 
					<< "\t" << it->WaitingTime 
					<< "\t" << it->BurstTime + it->WaitingTime
					<< "\t" << it->ArrivalTime
					<< endl;
				
				count++;		
				wTotal += it->WaitingTime;
				tTotal += it->BurstTime + it->WaitingTime;
			}
		}
		
	}
	
	int wAve = 0;
	int tAve = 0;
	
	if (count > 0) {
		wAve = wTotal / count;
		tAve = tTotal / count;
	}
	
	cout << "Average:\t" << wAve << "\t" << tAve << endl;
	cout << "Gantt Chart: ";
	
	for (list<char>::iterator it = cpu.Gantt.begin();
			it != cpu.Gantt.end();
			++it) {
		cout << *it;
	}
	
	cout << endl;
}*/
