#include "HuffmanCoding.h"

HuffmanCoding::HuffmanCoding(vector<char>& chars, vector<double>& probs)
{
	if (chars.size()!=probs.size()) return;
	for (unsigned i=0; i<chars.size(); ++i)
	{
		nodes.push_back(new Node);
		nodes[nodes.size()-1]->probablity=probs[i];
		nodes[nodes.size()-1]->c=chars[i];
		
		nodes[nodes.size()-1]->right=nodes[nodes.size()-1]->left=NULL;
		nodes[nodes.size()-1]->code="";
		
	}
	sort(nodes.begin(), nodes.end(), comparer);
}

HuffmanCoding::~HuffmanCoding(void)
{
	vector<Node*> nodeList;
	for (unsigned i=0; i<nodes.size(); ++i)
		nodeList.push_back(nodes[i]);
	int size = nodeList.size();
	for (int i=0; i<size; ++i)
	{
		queue<Node*> Q;
		Q.push(nodeList[i]);
		while (Q.size())
		{
			Node *temp;
			temp=Q.front();
			nodeList.push_back(temp);
			Q.pop();
			if (temp->left!=NULL)
				Q.push(temp->left);
			if (temp->right!=NULL)
				Q.push(temp->right);
		}
	}
	for (int i=0; i<size; ++i)
		for (int j=i+1; j<size; ++j)
		{
			if (nodeList[j]==nodeList[i])
				nodeList[j]=NULL;
			delete nodeList[i];
		}	
}

void HuffmanCoding::getCodes(vector<string>& cds, const vector<char>& chars)
{
	generateCodes();
	int size = chars.size();
	for (int i=0; i<size; ++i)
		for (int j=0; j<size; ++j)
			if (codes[j].c==chars[i])
			{
				cds.push_back(codes[j].code);
				continue;
			}
}

void HuffmanCoding::generateCodes()
{
	while (nodes.size()>1)
	{
		Node *temp = nodes[0];
		nodes[0] = new Node;
		nodes[0]->c=0;
		nodes[0]->probablity=temp->probablity+nodes[1]->probablity;
		nodes[0]->left = new Node;
		*(nodes[0]->left)=*temp;
		nodes[0]->right = new Node;
		*(nodes[0]->right)=*nodes[1];
		delete temp;
		delete nodes[1];
		nodes.erase(nodes.begin()+1);
		sort(nodes.begin(), nodes.end(), comparer);
	}
	queue<Node*> Q;
	Q.push(nodes[0]);
	while (Q.size())
	{
		Node *temp = Q.front();
		Q.pop();
		if (temp->left==NULL && temp->right==NULL)
		{
			Element tmp;
			tmp.code=temp->code;
			tmp.c=temp->c;
			codes.push_back(tmp);
			continue;
		}
		if (temp->right!=NULL)
			(temp->right->code=temp->code).append("1"), Q.push(temp->right);
		if (temp->left!=NULL)
			(temp->left->code=temp->code).append("0"), Q.push(temp->left);
		
	}
}