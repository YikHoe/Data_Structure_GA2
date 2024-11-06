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
	void push(string word, int weight);
	spamWords* getTop();
	void pop();
	bool isFull();
	bool isEmpty();
	bool detectSpam(const string& text);
};




#endif