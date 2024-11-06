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

bool spamDetector::isFull() {
    return sizeCnt == INT_SIZE;
}

bool spamDetector::isEmpty() {
    return sizeCnt == 0;
}

void spamDetector::push(string word, int weight) {
    if (isFull()) {
        resize();
    }

    sw[sizeCnt].word = word;
    sw[sizeCnt].susWeight = weight;
    sizeCnt++;
}

void spamDetector::pop() {
    if (!isEmpty()) {
        sw[sizeCnt].word = "";
        sw[sizeCnt].susWeight = 0;
        sizeCnt--;
        return;
    }

    cout << "Stack Underflow!" << endl;
}

spamWords* spamDetector::getTop() {
    if (isEmpty()) {
        cout << "Stack is empty!" << endl;
        return nullptr;
    }

    return &sw[sizeCnt];
}

bool spamDetector::detectSpam(const string& text) {
    int currentSusScore = 0;

    for (int i = 0; i < sizeCnt; i++) {
        if (text.find(sw[i].word) != string::npos) {
            currentSusScore += sw[i].susWeight;
        }
    }

    if (currentSusScore > 10) {
        return true;
    }
    else {
        return false;
    }

}

