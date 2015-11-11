#include <iostream>
#include <iomanip>
#include "dmm.h"

using namespace std;

const unsigned int allocBit = (unsigned int)(1<<31);

int GetSize(WORD *Word)
{
	int size = (*Word << 1) >> 1;
	return size;
};

bool IsAllocated(WORD *Word)
{
	bool allocated = (*Word >> 31) > 0;	
	return allocated;
};

DMM::DMM()
{
	currentBlock = 0;
	blockNo = 0;
	wordNo = 0;
};

/// Passed address of the memory to be used
void DMM::SetMemory(WORD *Memory, int Words)
{
	memory = Memory;
	
	// unallocated
	for (int i = 0; i < Words; i++)
		memory[i] = 0;

	// memory
	memory[0] = Words-2;
	memory[Words-1] = Words-2;
	
	AddBlock(&memory[0]);
	
	currentBlock = 0;
		
	words = Words;
};

/// Sets the allocation scheme to be used for allocating memory
void DMM::SetPolicy(Policy Type)
{
	type = Type;
};

/// Passed an integer representing the size, returns memory address of memory as a word*
WORD* DMM::Malloc(int Size)
{
	switch (type)
	{
		case eNone:
			cerr << "Policy set to None." << endl;
			break;
		case eFirstFit:
		
			for (vector<Block>::iterator it = blocks.begin();
					it != blocks.end();
					++it)
			{
				WORD *next = it->address;
				
				if (!IsAllocated(next) &&
					GetSize(next) >= Size) 
				{
					Allocate(next, Size);
					return next;
				}	
			}
			
			break;
		case eNextFit:
			{
				int i = blocks.size();
				currentBlock %= blocks.size();
				vector<Block>::iterator it = blocks.begin() + currentBlock;
				while (i > 0) 
				{
					WORD *next = it->address;
					
					if (!IsAllocated(next) &&
						GetSize(next) >= Size) 
					{
						Allocate(next, Size);
						return next;
					}	
					
					++it;
					if (it == blocks.end())
						it = blocks.begin();
					
					i--;
					currentBlock++;
					currentBlock %= blocks.size();
				};
			}
			break;
		case eBestFit:
			{
				int closestSize = -1;
				WORD* closestAddress = NULL;
				
				for (vector<Block>::iterator it = blocks.begin();
						it != blocks.end();
						++it)
				{
					WORD *next = it->address;
					
					if (!IsAllocated(next) &&
						GetSize(next) >= Size) 
					{
						if (closestAddress == NULL ||
							closestSize > GetSize(next) - Size)
						{
							closestSize = GetSize(next) - Size;
							closestAddress = next;
						}
					}	
				}
				
				if (closestAddress != NULL)
					Allocate(closestAddress, Size);
				return closestAddress;
			}			
		case eWorstFit:
			{
				int furthestSize = -1;
				WORD* furthestAddress = NULL;
				
				for (vector<Block>::iterator it = blocks.begin();
						it != blocks.end();
						++it)
				{
					WORD *next = it->address;
					
					if (!IsAllocated(next) &&
						GetSize(next) >= Size) 
					{
						if (furthestAddress == NULL ||
							furthestSize < GetSize(next) - Size)
						{
							furthestSize = GetSize(next) - Size;
							furthestAddress = next;
						}
					}	
				}
				
				if (furthestAddress != NULL)
					Allocate(furthestAddress, Size);
				return furthestAddress;
			}
	}	
	
	return NULL;
};

WORD* DMM::Calloc(int Size)
{
	WORD* word = Malloc(Size);
	
	if (word != NULL)
	{
		WORD* current = word;
		while (*(++current) != *word)
		{
			*current = 0;
		}
	}
	
	return word;
};

WORD* DMM::Realloc(WORD *Addr, int Size)
{
	WORD* word = Malloc(Size);
	
	// copy data
	int oldSize = GetSize(Addr);
	for (int i = 1; i <= oldSize; i++)
	{
		*(word + i) = *(Addr + i);
	}
	
	// Free
	Free(Addr);
	
	return word;
};

void DMM::Free(WORD *Addr)
{	
	int size = GetSize(Addr);
	
	if (IsAllocated(Addr)) 
	{
		if (Addr != &memory[0] && !IsAllocated(Addr-1)) // expand left
		{
			int size2 = GetSize((Addr-1));
			
			*(Addr-2-size2) = *(Addr+size+1) = size2+size+2;
			*(Addr-1) = *Addr = 0;
			
			RemoveBlock(Addr);
			
			size = size + size2 + 2;
			Addr = (Addr-2-size2);
		}
		
		if ((Addr+size+1) != &memory[words-1] && !IsAllocated(Addr+size+2)) // expand right
		{
			int size2 = GetSize(Addr+size+2);
			
			*(Addr+size+2) = *(Addr+size+1) = 0;
			*(Addr+size+size2+3) = *Addr = size + size2 + 2;
			
			RemoveBlock((Addr+size+2));
		}
		
		if (IsAllocated(Addr)) // still allocated 
		{
			*Addr -= allocBit;
			*(Addr + size + 1) -= allocBit;
		}
	}
};

void DMM::PrintAllocatedMemory()
{
	cout << "Position    Size     State" << endl;
	
	for (int i = 0; i < words; i++)
	{
		if (memory[i] != 0) 
		{
			bool alloc = IsAllocated(&memory[i]);
			
			if (alloc) 
			{
				cout << setw(6) << i << setw(10);
				cout << GetSize(&memory[i]) << "     ";
				cout << "Allocated";
				cout << endl;
			
				i += GetSize(&memory[i]) + 1;
			}	
		}	
	}
};

void DMM::PrintFreeMemory()
{	
	cout << "Position    Size     State" << endl;
	
	for (int i = 0; i < words; i++)
	{
		if (memory[i] != 0) 
		{
			bool alloc = IsAllocated(&memory[i]);
			
			if (!alloc) 
			{
				cout << setw(6) << i << setw(10);
				cout << GetSize(&memory[i]) << "     ";
				cout << "Free";
				cout << endl;
			
				i += GetSize(&memory[i]) + 1;
			}	
		}	
	}
};

void DMM::PrintAllMemory()
{
	cout << "Position    Size     State" << endl;
	
	for (int i = 0; i < words; i++)
	{
		if (memory[i] != 0) 
		{
			cout << setw(6) << i << setw(10);
			cout << GetSize(&memory[i]) << "     ";
			cout << (IsAllocated(&memory[i]) ? "Allocated" : "Free");
			cout << endl;
			
			i += GetSize(&memory[i]) + 1;	
		}	
	}
};

void DMM::PrintBlock(WORD *Addr)
{
	printf("%08x", *Addr);
};


/// return block
WORD* DMM::GetBlockAddress(int Index)
{
	for (std::vector<Block>::iterator it = blocks.begin();
				it != blocks.end();
				++it)
	{
		if (it->blockNo == Index) 
			return it->address;
	}	
	return NULL;	
};

int DMM::GetBlockNo(WORD* word)
{
	for (std::vector<Block>::iterator it = blocks.begin();
				it != blocks.end();
				++it)
	{
		if (it->address == word) 
			return it->blockNo;
	}	
	return -1;	
};

void DMM::RemoveBlock(WORD* word)
{
	for (std::vector<Block>::iterator it = blocks.begin();
				it != blocks.end();
				++it)
	{
		if (it->address == word) 
		{
			blocks.erase(it);
			break;
		}
	}
};

void DMM::AddBlock(WORD* address)
{
	Block block;
	block.blockNo = blockNo;
	block.address = address;
	blocks.push_back(block);
	
	blockNo++;
};

void DMM::PrintTotals()
{
	switch (type) {
		case eFirstFit:
			cout << "First Fit";
			break;
		case eNextFit:
			cout << "Next Fit";
			break;
		case eBestFit:
			cout << "Best Fit";
			break;
		case eWorstFit:
			cout << "Worst Fit";
			break;
	}
	
	cout << "\t       " << blockNo << "\t\t       " << wordNo << endl;
}

void DMM::Allocate(WORD *Word, int Size)
{
	int size = GetSize(Word);
	if (size > Size * 2) {	
							
		*Word = Size + allocBit;
		*(Word+Size+1) = Size + allocBit;
		*(Word+Size+2) = size - Size - 2;
		*(Word+size+1) = size - Size - 2;
		
		// remove old block
		RemoveBlock(Word);
		
		// add new blocks
		AddBlock(Word);
		AddBlock((Word+Size+2));
		
		wordNo += Size;
	}
	else {
		*Word = *Word + allocBit;
	}
};
