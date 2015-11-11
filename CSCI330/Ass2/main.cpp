/****************************************************************

Author: 	Shannon Archer
Student#:   4220912
Ass#:		Ass2
Due Date:	17/09/2015

1: Average Allocation Speed
Next Fit
First Fit
Best Fit / Worst Fit

2: Memory Utilization
Best Fit
First Fit
Next Fit
Worst Fit

****************************************************************/

#include <iostream>
#include <string>
#include <stdlib.h>
#include "dmm.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc == 1) 
	{
		cout << "Task 1: First Fit" << endl;
		
		/// task 1
		DMM MyDMM;
					
		int memory;		
		cout << "\nEnter memory size (WORDS): ";
		cin >> memory;
		
		WORD *Buff = new WORD[memory];
		MyDMM.SetMemory(Buff, memory);
		MyDMM.SetPolicy(eFirstFit);
		
		cout << "\nMMU memory assigned " << memory << " WORDS of memory" << endl;
		
		string command;
		int commandArg;
			
		bool quit = false;
		while (!quit) {
			
			cout << "Enter Command: ";
			cin >> command;
			
			if (command.compare("malloc") == 0)
			{
				cin >> commandArg;
				WORD* word = MyDMM.Malloc(commandArg);
				
				if (word == NULL) 
					cout << "Not able to allocate memory." << endl;
				else 
					cout << commandArg << " words allocated at block " 
						<< MyDMM.GetBlockNo(word) << endl;
			} 
			else if (command.compare("calloc") == 0)
			{
				cin >> commandArg;
				WORD* word = MyDMM.Calloc(commandArg);
				
				if (word == NULL) 
					cout << "Not able to allocate memory." << endl;
				else 
					cout << commandArg << " words allocated and cleared at block " 
						<< MyDMM.GetBlockNo(word) << endl;
			}
			else if (command.compare("realloc") == 0)
			{
				int block;
				cin >> block >> commandArg;
				WORD* realloc = MyDMM.GetBlockAddress(block);
				
				if (realloc == NULL)
					cout << "Block " << block << " does not exist." << endl;
				else {
					WORD* word = MyDMM.Realloc(realloc, commandArg);
					
					if (word == NULL) 
						cout << "Not able to allocate memory." << endl;
					else 
						cout << "Block " << block
							<< " reallocated to " << commandArg << " WORDS"
							<< " at block " << MyDMM.GetBlockNo(word) << endl;
				}
			}				
			else if (command.compare("free") == 0)
			{
				cin >> commandArg;
				WORD* word = MyDMM.GetBlockAddress(commandArg);
				if (word != NULL) {			
					MyDMM.Free(word);
					cout << "Block " << commandArg << " freed." << endl;
				} else {
					cout << "Block " << commandArg << " does not exist." << endl;
				}
			}
			else if (command.compare("pmem") == 0)
			{
				cout << endl;
				MyDMM.PrintAllMemory();
				cout << endl;
			}
			else if (command.compare("pfree") == 0)
			{
				cout << endl;
				MyDMM.PrintFreeMemory();
				cout << endl;
			}
			else if (command.compare("palloc") == 0)
			{
				cout << endl;
				MyDMM.PrintAllocatedMemory();
				cout << endl;
			}
			else if (command.compare("pblock") == 0)
			{
				cin >> commandArg;
				MyDMM.PrintBlock(MyDMM.GetBlockAddress(commandArg));
				cout << endl;
			}
			else 
			{
				quit = true;
			}
		}
	}
	/// task 2
	else
	{
		cout << "Task 2" << endl;
		
		int Size, Seed, Max, N;
		Size = atoi(argv[1]);
		Seed = atoi(argv[2]);
		Max = atoi(argv[3]);
		N = atoi(argv[4]);
				
		cout << "Policy\t\tBlocks Allocated\tWords Allocated" << endl;
				
		for (int p = eFirstFit; p != eNone; p++)
		{
			Policy policy = static_cast<Policy>(p);
			
			DMM mmu;
			WORD *Buff = new WORD[Size];
			mmu.SetMemory(Buff, Size);
			mmu.SetPolicy(policy);
			
			int Sd = Seed;
			
			vector<WORD*> words;
			for (int i = 0; i < N; i++)
			{
				srand(Sd++);
				
				while (true)
				{
					int numChars = (rand() % Max + 1) * 4;
					WORD *addr = mmu.Malloc(numChars);
					if (addr == NULL) {
						break; // memory full
					}
					words.push_back(addr);
				}
				
				srand(Sd++);
				int M = words.size() / 2;
				for (int j = 0; j < M; j++)
				{
					int pos = rand() % words.size();
					mmu.Free(*(words.begin()+pos));
					swap(words[pos], words.back());
					words.pop_back();
				}
				
			}
			
			mmu.PrintTotals();
		}
	}
	
	return 0;
}
