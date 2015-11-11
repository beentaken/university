#include <iostream>
#include <ctime>

#include <fstream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

void get_max_words(ifstream& fin, int& wordlen, int& numwords);
int cmp(const void* a, const void* b);
int tcmp(const void* a, const void* b);

int main(int argc, char* argv[])
{

	if(argc != 5)
     {
                cerr << "usage: " << argv[0]
                        << " infile outfile timesfile ntests" << endl;
			exit(EXIT_FAILURE);
     }

	
/*
	deal with commandline args
	do any preprocessing (max 1 pass over file, space O(1))
	read words into initial array (1 pass over file)
*/
	// grab arguments
	int ntests = atoi(argv[4]);
	char* infile = argv[1];
	char* outfile = argv[2];
	char* timesfile = argv[3];

	// setup streams
	ifstream inStream(infile);
	ofstream outStream(outfile);
	ofstream timesStream(timesfile);

	// vector to hold words
	string* words;

	// read info from file
	int max_length = 0, max_words = 0;
	std::string word;
	if (inStream.is_open()) {

		// get word count and max length
		while (inStream >> word) {
			max_words++;
			if ((int)word.length() > max_length)
				max_length = (int)word.length();
		}

		// create array
		words = new string[max_words];

		inStream.clear();
		inStream.seekg(0, ios::beg);

		// read words into array
		int i = -1;
		while (inStream >> word && i < max_words) {
			i++;
			words[i] = word;
		}

	} else {
		cout << "No input file exists.\n";
		return 0;
	}
	
	// temp data structure declaration	
	vector<string> temp;
	temp.reserve(max_words);

	vector<string*> final;
	final.reserve(max_words);
	
/*
	At this point, everything should be in the initial array
	A temporary data structure should be declared but not filled
*/

	/*set up the timer*/	
	clockid_t cpu;
	timespec start, end;
	long* times = new long[ntests];
	if(clock_getcpuclockid(0, &cpu) != 0)
	{
		cerr << "Error: could not get cpu clock" << endl;
		exit(EXIT_FAILURE);
	}
		
	for(int i=0; i<ntests; i++)
	{
		/*start the timer*/
		if(clock_gettime(cpu, &start) == -1)
		{
			cerr << "Error: could not get start time" << endl;
			exit(EXIT_FAILURE);
		}

		/*copy from initial array to temporary data structure*/
		temp.assign(words, words + max_words);
		final.clear();

		/*sort data in temporary structure*/
		// final = radixSort(temp, max_length);
		vector<string*> tempv;
		tempv.reserve(temp.size());

		for (int ia = 0; ia < (int)temp.size(); ia++) {
			tempv.push_back(&temp.at(ia));
		}

		for (int ib = 0; ib < max_length+1; ib++) {

			vector< vector<string*> > bin(257);

			// chuck into bins
			for (int jb = 0; jb < (int)tempv.size(); jb++) {

				string* word = tempv[jb];

				if ((int)word->length() - 1 < ib) {
					bin[0].push_back(word);
				}
				else {
					bin[(int)word->at(word->length() - 1 - ib)].push_back(word);
				}
			}

			// clear sorted val
			tempv.clear();

			// iterate over bin and place unsorted back into queue to sort
			// and sorted into final vector
			for (int jb = 0; jb < (int)bin.size(); jb++) {
			
				for (int hb = 0; hb < (int)bin[jb].size(); hb++) {
				
					if (jb != 0)
						tempv.push_back(bin[jb][hb]);
					else 
						final.push_back(bin[jb][hb]);

				}
			}
		}


		/*stop timer*/
		if(clock_gettime(cpu, &end) == -1)
		{
			cerr << "Error: could not get end time" << endl;
			exit(EXIT_FAILURE);
		}

		/*time per test in nanoseconds*/
		times[i] = (end.tv_sec - start.tv_sec)*1000000000 + 
                        end.tv_nsec - start.tv_nsec;
	}
	/*output sorted temporary array*/
	for (int i = 0; i < (int)final.size(); i++) {
		outStream << *final[i];
		if ((i+1) % 10 == 0)
			outStream << "\n";
		else
			outStream << " ";
	}
	outStream.close();

	/*sort times array and output it*/
	std::sort(times, times + ntests);
	for (int i = 0; i < ntests; i++) {
		timesStream << times[i] << '\n';
	}
	timesStream.close();

	delete[] times;
}
