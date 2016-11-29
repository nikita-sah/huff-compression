#pragma once
#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>
using namespace std;

struct Pair
{
	char ch;
	string code;
};

class Compressor
{
private:
	bool done;
	short endBlanks;
	vector<Pair> map;
	vector<unsigned char> head;
	string bodyBuffer;
	string fromFile;
	string toFile;
	void createHead();
	void createBody();
	void clearBuffer();
	string loadBuffer(ifstream&);
public:
	Compressor(vector<Pair>&);
	~Compressor(void);
	void compress(string, string);
};