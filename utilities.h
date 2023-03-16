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
		string word;
		char c;
		for(int i = pos[tid]; i < pos[tid+1]; i++)
		{
			c = text[i];
			if (isspace(c))
			{
				if(!word.empty())
				{
					words[tid].push_back(word);
					word.clear();
				}
			}
			else if(isalpha(c))
			{
				word += c;
			}
		}
	}
	//reduction
	for(int i = 0; i < nthreads; i++)
	{
		for(auto w : words[i])
			wordsReduction.push_back(w);
	}
	return wordsReduction;
}

#endif /* UTILITIES_H_ */
