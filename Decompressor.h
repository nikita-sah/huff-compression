#pragma once
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "Compressor.h"
using namespace std;

class Decompressor
{
private:
	bool done;
	short endBlanks;
	int maxSize;
	vector<Pair> chrMap;
	vector<unsigned char> head;
	string bodyBuffer;
	string fromFile, toFile;
	void extract();
	string getFromBuffer(ifstream&, int);
	char getNext(ifstream&);
	string charToBin(unsigned char);
	int fileSize(const string);
public:
	Decompressor(void);
	~Decompressor(void);
	void decompress(string, string);
	void testFile(ostream&);
};

