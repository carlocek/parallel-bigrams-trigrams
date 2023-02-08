/*
 * parallel.cpp
 *
 *  Created on: 23 gen 2023
 *      Author: carlo
 */
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <map>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <ctime>

using namespace std;

vector<string> tokenizeCharsNgrams(string text, int n)
{
	vector<string> tokenized;
	for(int i = 0; i < text.size()-(n-1); i++)
	{
		tokenized.push_back(text.substr(i, n));
	}
	return tokenized;
}

vector<string> tokenizeWords(string text)
{
	vector<string> words;
	string word;
	for(char c : text)
	{
		if (::isspace(c) != 0)
		{
			if (!word.empty())
			{
				words.push_back(word);
				word.clear();
			}
		}
		else
		{
			word += c;
		}
	}
	return words;
}

vector<string> bigrammizeWords(vector<string> words)
{
	vector<string> tokenized;
	for(int i = 0; i < words.size()-1; i++)
	{
		tokenized.push_back(words[i] + " " + words[i+1]);
	}
	return tokenized;
}

vector<string> trigrammizeWords(vector<string> words)
{
	vector<string> tokenized;
	for(int i = 0; i < words.size()-2; i++)
	{
		tokenized.push_back(words[i] + " " + words[i+1] + " " + words[i+2]);
	}
	return tokenized;
}

int main()
{
	//ifstream file("prova.txt");
	ifstream file("HeartOfDarkness-JosephConrad.txt");
	stringstream buffer;
	buffer << file.rdbuf();
	string text = buffer.str();
	map<string, int> bCharsFreq;
	map<string, int> tCharsFreq;
	map<string, int> bWordsFreq;
	map<string, int> tWordsFreq;

	chrono::high_resolution_clock::time_point t1, t2;
	chrono::duration<double> time_span;

	//format text to remove punctuation, upper case letters and eventually spaces
	text.erase(remove_if(text.begin(), text.end(), ::ispunct), text.end());
	for_each(text.begin(), text.end(), [](char& c) {c = tolower(c);});
	//use textSpaced for bigrams/trigrams of words
	string textSpaced = text;
	//use text for bigrams/trigrams of chars
	text.erase(remove_if(text.begin(), text.end(), ::isspace), text.end());

	//divides text in vectors of bigrams/trigrams of chars
	vector<string> bigramsChars = tokenizeCharsNgrams(text, 2);
	vector<string> trigramsChars = tokenizeCharsNgrams(text, 3);

	//divides text in vectors of bigrams/trigrams of words
	vector<string> words = tokenizeWords(textSpaced);
	vector<string> bigramsWords = bigrammizeWords(words);
	vector<string> trigramsWords = trigrammizeWords(words);

	//initialize maps for bigrams/trigrams of chars
	for(auto &b : bigramsChars)
	{
		bCharsFreq[b] = 0;
	}
	for(auto &t : trigramsChars)
	{
		tCharsFreq[t] = 0;
	}

	//initialize maps for bigrams/trigrams of words
	for (auto &b : bigramsWords)
	{
	    bWordsFreq[b] = 0;
	}
	for (auto &t : trigramsWords)
	{
		tWordsFreq[t] = 0;
	}

	int nthreads;
	vector<chrono::duration<double>> parallelTimes;

	for(nthreads = 1; nthreads < 33; nthreads++)
	{
		omp_set_dynamic(0);
		omp_set_num_threads(nthreads);

		t1 = chrono::high_resolution_clock::now();
		#pragma omp parallel default(none) shared(bigramsChars, trigramsChars, bigramsWords, trigramsWords, bCharsFreq, tCharsFreq, bWordsFreq, tWordsFreq)
		{
			#pragma omp for
			for (int i = 0; i < bigramsChars.size(); i++)
			{
				string bigram = bigramsChars[i];
				#pragma omp atomic
				bCharsFreq[bigram]++;
			}
			#pragma omp for
			for (int i = 0; i < trigramsChars.size(); i++)
			{
				string trigram = trigramsChars[i];
				#pragma omp atomic
				tCharsFreq[trigram]++;
			}

			#pragma omp for
			for (int i = 0; i < bigramsWords.size(); i++)
			{
				string bigram = bigramsWords[i];
				#pragma omp atomic
				bWordsFreq[bigram]++;
			}
			#pragma omp for
			for (int i = 0; i < trigramsWords.size(); i++)
			{
				string trigram = trigramsWords[i];
				#pragma omp atomic
				tWordsFreq[trigram]++;
			}
		}
		t2 = chrono::high_resolution_clock::now();
		time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
		parallelTimes.push_back(time_span);
		printf("\n elapsed time: %f\n", time_span.count());
		fflush(stdout);
	}

	for(auto &e : parallelTimes)
		printf("%f,", e);

	/*for(auto &elem : bCharsFreq)
		cout << elem.first << " --> " << elem.second << endl;
	for(auto &elem : tCharsFreq)
		cout << elem.first << " --> " << elem.second << endl;
	for(auto &e : bWordsFreq)
		cout << e.first << " --> " << e.second << endl;*/


	return 0;
}







