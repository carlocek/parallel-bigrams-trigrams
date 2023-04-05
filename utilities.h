#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <string>
#include <vector>
#include <omp.h>

using namespace std;

vector<string> sequentialTokenizeWords(string& text)
{
	vector<string> words;
	string word;
	for(char c : text)
	{
		if(isspace(c))
		{
			if(!word.empty())
			{
				words.push_back(word);
				word.clear();
			}
		}
		else if(isalpha(c))
		{
			word += c;
		}
	}
	return words;
}

vector<string> parallelTokenizeWords(string& text, int textLength, int nthreads)
{
	//compute array of text positions to divide iterations between threads
	int pos[nthreads+1];
	pos[0] = 0;
	pos[nthreads] = textLength;
	for(int i = 1; i < nthreads; i++)
	{
		pos[i] = i * (textLength/nthreads);
		while(!isspace(text[pos[i]-1]))
		{
			pos[i]++;
		}
	}
	//declaration of global vector
	vector<string> wordsReduction;
	//declaration of private vector to be reduced
	vector<string> words[nthreads];
	#pragma omp parallel default(none) shared(text, pos, words, wordsReduction)
	{
		int tid = omp_get_thread_num();
		int start = pos[tid];
		int end = pos[tid+1];
		string word;
		int j = 0;
		char c;
		for(int i = start; i < end; i++)
		{
			c = text[i];
			if(isspace(c))
			{
				if(!word.empty())
				{
					//resize adding the space needed for the word to be added then insert to avoid multiple memory reallocations
					words[tid].resize(words[tid].size() + word.size());
					words[tid][j].insert(words[tid][j].end(), word.begin(), word.end());
					j++;
					word.clear();
				}
			}
			else if(isalpha(c))
			{
				word += c;
			}
		}
		words[tid].resize(j);
	}
	//reduction phase
	for(int i = 0; i < nthreads; i++)
	{
		//resize and insert whole vector at every iteration
		wordsReduction.resize(wordsReduction.size() + words[i].size());
		wordsReduction.insert(wordsReduction.end(), words[i].begin(), words[i].end());
	}
	return wordsReduction;
}

#endif /* UTILITIES_H_ */
