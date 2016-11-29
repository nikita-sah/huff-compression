#pragma once
#include <vector>
using namespace std;

class CountingSet
{
private:
	struct Element
	{
		char c;
		int count;
	};
	vector<Element> ems;
public:
	CountingSet(void);
	~CountingSet(void);
	void addElement(char);
	void removeLast();
	void getPercentages(vector<double>&);
	void getCharacters(vector<char>&);
};

