#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;

class HuffmanCoding
{
private:
	struct Element
	{
		char c;
		string code;
	};

	struct Node
	{
		char c;
		string code;
		double probablity;
		Node *left, *right;
	};

	struct comparison
	{
		bool operator() (Node*& i, Node*& j) 
		{ 
			return (i->probablity<j->probablity);
		}
	} comparer;

	vector<Element> codes;
	
	Node *root;
	vector<Node*> nodes;
	void generateCodes();
public:
	void getCodes(vector<string>&, const vector<char>&);
	HuffmanCoding(vector<char>&, vector<double>&);
	~HuffmanCoding(void);
	
};

