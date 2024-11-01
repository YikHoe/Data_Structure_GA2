#include "spamDetector.hpp"

spamDetector::spamDetector() {
	sw = new spamWords[INT_SIZE];
	sizeCnt = 0;
	cap = INT_SIZE;
}

spamDetector::~spamDetector() {
	delete[] sw;
}

void spamDetector::resize() {
	cap *= 2;

    spamWords* newArray = new spamWords[cap];

    for (int i = 0; i < sizeCnt; i++) {
        newArray[i] = sw[i];
    }

    delete[] sw;
    sw = newArray;
}

void spamDetector::insert(string word, int weight) {
    if (sizeCnt == cap) {
        resize();
    }

    sw[sizeCnt].word = word;
    sw[sizeCnt].susWeight = weight;
    sizeCnt++;
}

int spamDetector::detectSpam(const string& text) {
    int currentSusScore = 0;

    for (int i = 0; i < sizeCnt; i++) {
        if (text.find(sw[i].word) != string::npos) {
            currentSusScore += sw[i].susWeight;
        }
    }

    return currentSusScore;
}

