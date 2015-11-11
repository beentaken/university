enum AlgType { eRandom, eFIFO, eLRU, eLFU, eOptimal, eSecondChance, eNone };


struct Frame
{
	int faults, job, access;
};

class Pager {
	public:
		Pager();
		void Init(AlgType alg, int numFrames, int* reference, int referenceSize);
		// Returns true if major page fault occurs
		bool AccessPage(int pageNum);
		int GetPageFaults();
	private:
		// 
		AlgType Alg;
		int NumFrames;	
		int* Reference;	
		int ReferenceSize;
		int CurrentFrame;
		Frame* Frames;
		
		int PageFaults;
		
		int GetJobIndex(int job);
		bool IsFull();
		void ReplaceCurrent(int job);
};
