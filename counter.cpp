#include "counter.h"

CountingSet::CountingSet(void) { }

CountingSet::~CountingSet(void) { }

void CountingSet::addElement(char e)
{
	for (unsigned i=0; i<ems.size(); ++i)
		if (ems[i].c==e)
		{
			++ems[i].count;
			return;
		}
	Element temp;
	temp.c=e;
	temp.count=1;
	ems.push_back(temp);
}

void CountingSet::getPercentages(vector<double>& percs)
{
	int count=0;
	for (unsigned i=0; i<ems.size(); ++i)
	{
		count+=ems[i].count;
		percs.push_back(ems[i].count*100);
	}
	for (unsigned i=0; i<ems.size(); ++i)
		percs[i]/=count;
}

void CountingSet::getCharacters(vector<char>& chars)
{
	for (unsigned i=0; i<ems.size(); ++i)
		chars.push_back(ems[i].c);
}

void CountingSet::removeLast()
{
	ems.pop_back();
}