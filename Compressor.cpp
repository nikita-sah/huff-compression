#include "Compressor.h"

Compressor::Compressor(vector<Pair>& inMap)
{
	for (unsigned i=0; i<inMap.size(); ++i)
		map.push_back(inMap[i]);
	bodyBuffer="";
	bodyBuffer.reserve(512);
	done=false;
	endBlanks=0;
}

Compressor::~Compressor(void) { }

void Compressor::clearBuffer()
{
	if (bodyBuffer.size()>256)
	{
		string temp = bodyBuffer.substr(256);
		bodyBuffer=temp;
	}
	if (bodyBuffer.size()==256)
	{
		bodyBuffer="";
	}
	if (done)
		while (bodyBuffer.size()%8!=0)
			bodyBuffer+="0", ++endBlanks;
}

void Compressor::compress(string from, string to)
{
	fromFile=from;
	toFile=to;
	createHead();
	createBody();
}

void Compressor::createHead()
{
	head.push_back(0);
	head.push_back(map.size());
	for (unsigned i=0; i<map.size(); ++i)
		head.push_back(map[i].ch);
	for (unsigned i=0; i<map.size(); ++i)
		head.push_back(map[i].code.size());
	for (unsigned i=0; i<map.size(); ++i)
		bodyBuffer+=map[i].code;
}

string Compressor::loadBuffer(ifstream& strm)
{
	if (bodyBuffer.size()<256)
	{
		char chr;
		while (bodyBuffer.size()<256)
		{
			chr = strm.get();
			if (strm.eof()) break;
			for (unsigned j=0; j<map.size(); ++j)
				if (map[j].ch==chr)
				{
					bodyBuffer+=map[j].code;
					break;
				}
		}
		if (strm.eof())	done=true;
		if (done)
		{
			clearBuffer();
			return bodyBuffer;
		}
	}
	string temp="";
	temp.reserve(256);
	for (int i=0; i<256; ++i)
		temp+=bodyBuffer[i];
	clearBuffer();
	return temp;
}

void Compressor::createBody()
{
	ofstream file;
	ifstream dat;
	file.open(toFile.c_str(), ios::out|ios::binary);
	dat.open(fromFile.c_str(), ios::in|ios::binary);
	if (!file.is_open()) return;
	for (unsigned i=0; i<head.size(); ++i)
	{
		char buffer[1];
		buffer[0]=head[i];
		file.write(buffer,1);
	}
	while (!done)
	{
		string tmp = loadBuffer(dat);
		char buffer[256];
		int intBuffer[256];
		for (unsigned i=0; i<tmp.size()/8; ++i)
			intBuffer[i]=0;
		for (unsigned i=0; i<tmp.size(); i+=8)
			for (unsigned j=i; j<i+8; ++j)
			{
				if (tmp[j]=='1')
					intBuffer[i/8]+=(int)pow((double)2,(double)(7-j%8));
			}	
		for (unsigned i=0; i<tmp.size(); ++i)
			buffer[i]=intBuffer[i];
		file.write(buffer, tmp.size()/8);
		file.flush();
	}
	file.seekp(ios::beg);
	char buffer[1];
	buffer[0]=(char)endBlanks;
	file.write(buffer, 1);
	file.close();
	dat.close();
}