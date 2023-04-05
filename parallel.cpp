#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <unordered_map>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <ctime>
#include <numeric>
#include <cmath>
#include "utilities.h"

using namespace std;

int main()
{
//	ifstream file1("prova.txt");
	ifstream file1("books/GreatExpectations.txt");//1MB
	ifstream file2("books/CrimeAndPunishment.txt");//1.14MB
	ifstream file3("books/DonQuixote.txt");//2.28MB
	ifstream file4("books/TheCountOfMontecristo.txt");//2.65MB
	ifstream file5("books/WarAndPeace.txt");//3.20MB
	stringstream buffer1, buffer2, buffer3, buffer4, buffer5;
	buffer1 << file1.rdbuf();
	file1.close();
	buffer2 << file2.rdbuf();
	file2.close();
	buffer3 << file3.rdbuf();
	file3.close();
	buffer4 << file4.rdbuf();
	file4.close();
	buffer5 << file5.rdbuf();
	file5.close();

//	string text = buffer1.str();
	string text = buffer1.str() + buffer2.str() + buffer3.str() + buffer4.str() + buffer5.str();

	chrono::high_resolution_clock::time_point t1, t2;
	chrono::duration<double> time_span;

	int nthreads = 8;
	omp_set_dynamic(0);
	omp_set_num_threads(nthreads);

	//declaration of global maps
	unordered_map<string, int> bcFreqReduction;
	unordered_map<string, int> tcFreqReduction;
	unordered_map<string, int> bwFreqReduction;
	unordered_map<string, int> twFreqReduction;
	//declaration of private maps to be reduced
	unordered_map<string, int> bcFreq[nthreads];
	unordered_map<string, int> tcFreq[nthreads];
	unordered_map<string, int> bwFreq[nthreads];
	unordered_map<string, int> twFreq[nthreads];

	int textLength = text.size();
	cout << textLength << endl;

	for(int it = 0; it < 20; it++)
	{
	t1 = chrono::high_resolution_clock::now();

	//compute words vector
	vector<string> wordsReduction = parallelTokenizeWords(text, textLength, nthreads);
	int wordsLength = wordsReduction.size();

	#pragma omp parallel default(none) shared(text, textLength, wordsReduction, wordsLength, bcFreq, tcFreq, bwFreq, twFreq, bcFreqReduction, tcFreqReduction, bwFreqReduction, twFreqReduction)
	{
		int tid = omp_get_thread_num();
		int nth = omp_get_num_threads();
		int chunkc = textLength/nth;
		int chunkw = wordsLength/nth;
		int bcstart, tcstart, bwstart, twstart;
		int bcend, tcend, bwend, twend;
		//compute start and end points for each thread
		bcstart = tid*chunkc;
		tcstart = tid*chunkc;
		bwstart = tid*chunkw;
		twstart = tid*chunkw;
		if(tid == nth-1)
		{
			bcend = textLength-1;
			bwend = wordsLength-1;
			tcend = textLength-2;
			twend = wordsLength-2;
		}
		else
		{
			bcend = (tid+1)*chunkc;
			bwend = (tid+1)*chunkw;
			tcend = (tid+1)*chunkc;
			twend = (tid+1)*chunkw;
		}

		//private buffer for updating map of bigrams of chars
		string bcBuf;
		//private buffer for updating map of trigrams of chars
		string tcBuf;
		char cc; //current character
		//private buffer for updating map of bigrams of words
		string bwBuf;
		//private buffer for updating map of trigrams of words
		string twBuf;

		for(int i = bcstart; i < bcend; i++)
		{
			bcBuf = "";
			for(int j = 0; j < 2; j++)
			{
				cc = text[i+j];
				if(isalpha(cc))
					bcBuf += cc;
			}
			if(bcBuf.size() == 2)
				bcFreq[tid][bcBuf]++;
		}

		for(int i = tcstart; i < tcend; i++)
		{
			tcBuf = "";
			for(int j = 0; j < 3; j++)
			{
				cc = text[i+j];
				if(isalpha(cc))
					tcBuf += cc;
			}
			if(tcBuf.size() == 3)
				tcFreq[tid][tcBuf]++;
		}

		for(int i = bwstart; i < bwend; i++)
		{
			bwBuf = "";
			for(int j = 0; j < 2; j++)
			{
				bwBuf += wordsReduction[i+j];
				bwBuf += " ";
			}
			bwFreq[tid][bwBuf]++;
		}

		for(int i = twstart; i < twend; i++)
		{
			twBuf = "";
			for(int j = 0; j < 3; j++)
			{
				twBuf += wordsReduction[i+j];
				twBuf += " ";
			}
			twFreq[tid][twBuf]++;
		}

		#pragma omp barrier
		//reduction
		for(int i = 0; i < nth; i++)
		{
			#pragma omp sections
			{
				#pragma omp section
				{
				for(auto b : bcFreq[i])
					bcFreqReduction[b.first] += b.second;
				}
				#pragma omp section
				{
				for(auto t : tcFreq[i])
					tcFreqReduction[t.first] += t.second;
				}
				#pragma omp section
				{
				for(auto b : bwFreq[i])
					bwFreqReduction[b.first] += b.second;
				}
				#pragma omp section
				{
				for(auto t : twFreq[i])
					twFreqReduction[t.first] += t.second;
				}
			}
		}
	}
	t2 = chrono::high_resolution_clock::now();
	time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
	printf("\n elapsed time: %f\n", time_span.count());
	fflush(stdout);
	}

	//PRINT RESULTS
//	for(auto elem : bcFreqReduction)
//		cout << elem.first << " --> " << elem.second << endl;

//	for(auto &elem : tcFreqReduction)
//		cout << elem.first << " --> " << elem.second << endl;

//	for(auto &elem : bwFreqReduction)
//		cout << elem.first << " --> " << elem.second << endl;

//	for(auto &elem : twFreqReduction)
//		cout << elem.first << " --> " << elem.second << endl;


//	for(auto elem : bcFreqReduction)
//	{
//		if(elem.first == "in")
//			cout << elem.first << " --> " << elem.second << endl;
//	}
//	for(auto elem : tcFreqReduction)
//	{
//		if(elem.first == "the")
//			cout << elem.first << " --> " << elem.second << endl;
//	}
//	for(auto elem : bwFreqReduction)
//	{
//		if(elem.first == "in the ")
//			cout << elem.first << " --> " << elem.second << endl;
//	}
//	for(auto elem : twFreqReduction)
//	{
//		if(elem.first == "flood had made ")
//			cout << elem.first << " --> " << elem.second << endl;
//	}

	return 0;
}
