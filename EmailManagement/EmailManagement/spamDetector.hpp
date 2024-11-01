#pragma once
#ifndef SPAMDETECTOR_HPP
#define SPAMDETECTOR_HPP

#include <iostream>
#define INT_SIZE 1000

using namespace std;

struct spamWords
{
	string word;
	int susWeight;
};

class spamDetector
{
private:
	spamWords* sw;
	int sizeCnt;
	int cap;

	// methods
	void resize();

public:
	spamDetector();
	~spamDetector();
	void insert(string word, int weight);
	bool detectSpam(const string& text);
};




#endif