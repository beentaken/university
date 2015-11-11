#if !defined(DMM_H)
#define DMM_H

#include <vector>

typedef unsigned int WORD;
enum Policy { eFirstFit, eNextFit, eBestFit, eWorstFit, eNone };

extern int GetSize(WORD* word);
extern bool IsAllocated(WORD* word);

struct Block {
	int blockNo;
	WORD* address;
};

class DMM {
	private:
		WORD* memory;
		int words;
		Policy type;
		int blockNo;
		int wordNo;
		int currentBlock;
		std::vector<Block> blocks;
		// my helpers
		void AddBlock(WORD *Addr);
		void RemoveBlock(WORD *Addr);
		void Allocate(WORD *Addr, int Size);
	public:
		DMM();
		void SetMemory(WORD *Memory, int Words);
		void SetPolicy(Policy type);
		WORD *Malloc(int Size);
		WORD *Calloc(int Size);
		WORD *Realloc(WORD *Addr, int Size);
		void Free(WORD *Addr);
		void PrintAllocatedMemory();
		void PrintFreeMemory();
		void PrintAllMemory();
		void PrintBlock(WORD *Addr);
		void PrintTotals();
		// other helpers
		WORD *GetBlockAddress(int Index);
		int GetBlockNo(WORD* word);
};

#endif // !defined(DMM_H)
