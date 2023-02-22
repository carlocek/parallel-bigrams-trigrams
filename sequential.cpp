#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <ctime>

#include "utilities.h"

using namespace std;

int main()
{
	//ifstream file("prova.txt");
	ifstream file1("books/GreatExpectations.txt");
	ifstream file2("books/CrimeAndPunishment.txt");
	ifstream file3("books/DonQuixote.txt");
	ifstream file4("books/TheCountOfMontecristo.txt");
	ifstream file5("books/WarAndPeace.txt");
	stringstream buffer1, buffer2, buffer3, buffer4, buffer5;
	buffer1 << file1.rdbuf();
	file1.close();
//	string book1 = buffer1.str();
	buffer2 << file2.rdbuf();
	file2.close();
//	string book2 = buffer2.str();
	buffer3 << file3.rdbuf();
	file3.close();
//	string book3 = buffer3.str();
	buffer4 << file4.rdbuf();
	file4.close();
//	string book4 = buffer4.str();
	buffer5 << file5.rdbuf();
	file5.close();
//	string book5 = buffer5.str();

	string text = buffer1.str() + buffer2.str() + buffer3.str() + buffer4.str() + buffer5.str();

	unordered_map<string, int> bcFreq;
	unordered_map<string, int> tcFreq;
	unordered_map<string, int> bwFreq;
	unordered_map<string, int> twFreq;

	chrono::high_resolution_clock::time_point t1, t2;
	chrono::duration<double> time_span;

	int textLength = text.size();
	cout << textLength << endl;

	float sequentialTimes[10];
	for(int it = 0; it < 10; it++)
	{
	t1 = chrono::high_resolution_clock::now();

	vector<string> words = sequentialTokenizeWords(text);
	int wordsLength = words.size();
	//cout << wordsLength << endl;

	string bcBuf;
	string tcBuf;
	string bwBuf[2];
	string twBuf[3];
	char cc;

	for(int i = 0; i < textLength-1; i++)
	{
		bcBuf = "";
		for(int j = 0; j < 2; j++)
		{
			cc = text[i+j];
			if(isalpha(cc))
				bcBuf += cc;
		}
		if(bcBuf.size() == 2)
			bcFreq[bcBuf]++;
	}

	for(int i = 0; i < textLength-2; i++)
	{
		tcBuf = "";
		for(int j = 0; j < 3; j++)
		{
			cc = text[i+j];
			if(isalpha(cc))
				tcBuf += cc;
		}
		if(tcBuf.size() == 3)
			tcFreq[tcBuf]++;
	}

	for(int i = 0; i < wordsLength-1; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			bwBuf[j] = words[i+j];
		}
		bwFreq[bwBuf[0] + " " + bwBuf[1]]++;
	}

	for(int i = 0; i < wordsLength-2; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			twBuf[j] = words[i+j];
		}
		twFreq[twBuf[0] + " " + twBuf[1] + " " + twBuf[2]]++;
	}
	t2 = chrono::high_resolution_clock::now();
	time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
	printf("\n elapsed time: %f\n", time_span.count());
	fflush(stdout);
	}

	double sum = 0;
	for(int i = 0; i < 10; i++)
		sum += sequentialTimes[i];
	double avg = sum / 10;
	cout << avg << endl;

//	for(auto &elem : bcFreq)
//		cout << elem.first << " --> " << elem.second << endl;

//	for(auto &elem : tcFreq)
//		cout << elem.first << " --> " << elem.second << endl;

//	for(auto &elem : bwFreq)
//		cout << elem.first << " --> " << elem.second << endl;

//	for(auto &elem : twFreq)
//		cout << elem.first << " --> " << elem.second << endl;


//	for(auto elem : bcFreq)
//	{
//		if(elem.first == "in")
//			cout << elem.first << " --> " << elem.second << endl;
//	}
//	for(auto elem : tcFreq)
//	{
//		if(elem.first == "the")
//			cout << elem.first << " --> " << elem.second << endl;
//	}
//	for(auto elem : bwFreq)
//	{
//		if(elem.first == "in the")
//			cout << elem.first << " --> " << elem.second << endl;
//	}
//	for(auto elem : twFreq)
//	{
//		if(elem.first == "flood had made")
//			cout << elem.first << " --> " << elem.second << endl;
//	}

	return 0;
}

























