/****************************************************************

Author: 	Shannon Archer
Student#:   4220912
Ass#:		Ass2
Due Date:	20/10/2015

Performance

1. Optimal
2. 2nd Chance
3. FIFO
4. LFU
5. LRU
6. Random

****************************************************************/
#include <iostream>
#include <iomanip>
#include "pager.h"

using namespace std;

int main(int argc, char** argv)
{	
	bool RefSupplied = false;
	int Seed, MinFrames, MaxFrames, FrameInc, NumPages, NumPageRefs;
	int *reference;
		
	if (argc == 6)
	{
		RefSupplied = true;
		Seed = 0;
		MinFrames = atoi(argv[1]);
		MaxFrames = atoi(argv[2]);
		FrameInc = atoi(argv[3]);
		NumPageRefs = atoi(argv[4]);
				
		cout << "MinFrames: " << MinFrames << endl;
		cout << "MaxFrames: " << MaxFrames << endl;
		cout << "FrameInc: " << FrameInc << endl;
		
		cout << "RefString: ";		
		reference = new int[NumPageRefs];
		for (int i = 0; i < NumPageRefs; i++)
		{
			reference[i] = argv[5][i] - '0';
			cout << reference[i];	
		}
		cout << endl;
	}
	else if (argc == 7) 
	{
		Seed = atoi(argv[1]);
		MinFrames = atoi(argv[2]);
		MaxFrames = atoi(argv[3]);
		FrameInc = atoi(argv[4]);
		NumPages = atoi(argv[5]);
		NumPageRefs = atoi(argv[6]);
		
		cout << "Seed: " << Seed << endl;
		cout << "MinFrames: " << MinFrames << endl;
		cout << "MaxFrames: " << MaxFrames << endl;
		cout << "FrameInc: " << FrameInc << endl;
		cout << "NumPages: " << NumPages << endl;
		cout << "NumPageRefs: " << NumPageRefs << endl;
	} 
	else 
	{		
		cout << "Enter rand seed: ";
		cin >> Seed;
		
		cout << "Enter min no. of frames: ";
		cin >> MinFrames;
		
		cout << "Enter max no. of frames: ";
		cin >> MaxFrames;
		
		cout << "Enter frame increments: ";
		cin >> FrameInc;
			
		cout << "Enter no. of pages: ";
		cin >> NumPages;
		
		cout << "Enter no. of page refs: ";
		cin >> NumPageRefs;		
	}	
		
	if (MaxFrames - MinFrames < FrameInc) 
	{
		cerr << "Max frames must be greater then min frames by more than frame increment" << endl;
		return 1;
	}
	
	// seed rand
	srand(Seed);
	
	if (!RefSupplied)
	{
		// generate reference string
		reference = new int[NumPageRefs];
		// cout << "DEBUG: REFSTR = ";
		for (int i = 0; i < NumPageRefs; i++)
		{
			reference[i] = rand() % NumPages;
			// cout << reference[i] << ",";
		}
		// cout << endl;
	}
	
	// print headers
	cout << "\n\n\t\tPAGE FAULTS" << endl;
	cout << "Frames:\t";
	for (int i = MinFrames; i <= MaxFrames; i += FrameInc)
	{
		cout << setw(5) << i;
	}
	cout << setw(5) << "T";
	cout << endl;
	
	// run tests
	for (int a = eRandom; a != eNone; a++)
	{
		AlgType alg = static_cast<AlgType>(a);
				
		switch (alg)
		{
			case eRandom:
				cout << "Random";
				break;
			case eFIFO:
				cout << "FIFO";
				break;
			case eLRU:
				cout << "LRU";
				break;
			case eLFU:
				cout << "LFU";
				break;
			case eOptimal:
				cout << "Optimal";
				break;
			case eSecondChance:
				cout << "2ndCA";
				break;
			default:
				cout << "Error";
		}
		cout << "\t";
		
		for (int i = MinFrames; i <= MaxFrames; i += FrameInc)
		{		
			Pager pager;
			pager.Init(alg, i, reference, NumPageRefs);
			
			for (int i = 0; i < NumPageRefs; i++)
			{
				pager.AccessPage(i);
			}
			
			// print row
			cout << setw(5) << pager.GetPageFaults();
		}
		
		cout << endl;
	}	
	
	return 0;	
}
