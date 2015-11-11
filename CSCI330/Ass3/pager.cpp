#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "pager.h"

using namespace std;

Pager::Pager()
{
	// do nothing
}

void Pager::Init(AlgType alg, int numFrames, int* reference, int referenceSize)
{
	Alg = alg;
	NumFrames = numFrames;
	Reference = reference;
	ReferenceSize = referenceSize;
	CurrentFrame = 0;
	PageFaults = 0;
			
	Frames = new Frame[numFrames];
	for (int i = 0; i < numFrames; i++) 
	{
		Frames[i].faults = 0;
		Frames[i].access = 0;
		Frames[i].job = -1;
	}
}

bool Pager::AccessPage(int PageNum)
{	
	int Job = Reference[PageNum];
	bool majorFault = false, minorFault = false;
	
	AlgType alg = Alg;
	// Perform eFIFO if not full
	if (!IsFull() && alg != eSecondChance) {
		minorFault = true;
		alg = eFIFO;
	}
	
	// eRandom, eFIFO, eLRU, eLFU, eOptimal
	switch (alg)
	{
		case eRandom:					
			// check if job in frames
			if (GetJobIndex(Job) == -1) 
			{				
				// set current to random
				CurrentFrame = rand() % NumFrames;
				ReplaceCurrent(Job);
				
				majorFault = true;
			}
						
			break;
		case eFIFO:				
			// if job not currently assigned
			if (GetJobIndex(Job) == -1) 
			{				
				ReplaceCurrent(Job);						
				// increment current
				CurrentFrame = (CurrentFrame + 1) % NumFrames;	
				Frames[CurrentFrame].access = 1;
				
				if (!minorFault)
					majorFault = true;
			}			
					
			break;
		case eLRU:
		{
			int index = GetJobIndex(Job);
			if (index == -1)
			{
				// get least recently used
				for (int i = 0; i < NumFrames; i++)
				{
					if (Frames[i].access < Frames[CurrentFrame].access)
						CurrentFrame = i;
				}				
				index = CurrentFrame;
				ReplaceCurrent(Job);
				
				majorFault = true;
			}
			// update access time
			Frames[index].access = PageNum;
					
			break;
		}
		case eLFU:
		{
			int index = GetJobIndex(Job);
			if (index == -1)
			{
				// create array for frequency
				int* frequency = new int[NumFrames];
				for (int i = 0; i < NumFrames; i++)
					frequency[i] = 0;
				
				// get least frequency used
				for (int i = PageNum-1; i >= 0; i--)
				{
					int jobIndex = GetJobIndex(Reference[i]);
					frequency[jobIndex]++;
				}	
				
				// find least frequent
				for (int i = 1; i < NumFrames; i++)
				{
					if (frequency[i] < frequency[CurrentFrame]) 
						CurrentFrame = i;
					// cout << Frames[i].job << ":" << frequency[i] << " | ";
				}
				
				index = CurrentFrame;
				ReplaceCurrent(Job);
				// cout << endl;				
				majorFault = true;
			}
			
			break;
		}
		case eOptimal:
		{
			int index = GetJobIndex(Job);
			if (index == -1)
			{				
				// get best job to remove
				int* pre = new int[NumFrames];
				for (int i = 0; i < NumFrames; i++)
					pre[i] = 0;
				
				for (int i = PageNum-1; i < ReferenceSize; i++)
				{
					bool complete = false;
					
					int nextJob = Reference[i];
					pre[GetJobIndex(nextJob)]++; 
					
					int zeroes = 0;
					for (int j = 0; j < NumFrames; j++)
					{
						if (pre[j] == 0)
							zeroes++;
					}
					complete = (zeroes <= 1);
					
					if (complete)
						break;					
				}
				
				// optimum = first frame with 0
				for (int i = 0; i < NumFrames; i++)
				{					
					if (pre[i] == 0) 
					{
						CurrentFrame = i;
						break;
					}
				}
				
				ReplaceCurrent(Job);		
				
				majorFault = true;		
			}
		
			break;
		}
		case eSecondChance:
		{
			// if job not currently assigned
			int index = GetJobIndex(Job);
			if (index == -1) 
			{						
				bool found = false;
				while (!found)
				{
					if (Frames[CurrentFrame].access > 0) 
					{
						Frames[CurrentFrame].access = 0;
						CurrentFrame = (CurrentFrame + 1) % NumFrames;
					}
					else
					{
						found = true;
					}	
				}	
												
				ReplaceCurrent(Job);
				Frames[CurrentFrame].access = 1;
				CurrentFrame = (CurrentFrame + 1) % NumFrames;
				
				majorFault = true;
			}
			else
				Frames[index].access = 1;
					
			break;
		}
	}
	
	if (minorFault || majorFault)
		PageFaults++;
	
	return majorFault;
}

int Pager::GetPageFaults()
{
	return PageFaults;
}

int Pager::GetJobIndex(int job)
{
	for (int i = 0; i < NumFrames; i++)
	{
		if (Frames[i].job == job)
			return i;
	}
	return -1;
}

bool Pager::IsFull()
{
	for (int i = 0; i < NumFrames; i++)
	{
		if (Frames[i].job == -1)
			return false;
	}
	return true;
}

void Pager::ReplaceCurrent(int job)
{	
	Frames[CurrentFrame].job = job;
}
