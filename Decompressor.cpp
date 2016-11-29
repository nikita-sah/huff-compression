#include "Decompressor.h"

Decompressor::Decompressor(void) 
{
	bodyBuffer="";
	bodyBuffer.reserve(256);
	maxSize=0;
	done=false;
}

Decompressor::~Decompressor(void) { }

void Decompressor::extract()
{
	ifstream cFile;
	cFile.open(fromFile.c_str(),ios::in | ios::binary);
	if (!cFile.is_open()) return;
	endBlanks=cFile.get();
	int mapSize;
	mapSize=cFile.get();
	vector<int> codeSizes;
	for (int i=0; i<mapSize; ++i)
	{
		Pair temp;
		temp.ch=cFile.get();
		chrMap.push_back(temp);
	}
	for (int i=0; i<mapSize; ++i)
		codeSizes.push_back(cFile.get());
	for (int i=0; i<mapSize; ++i)
		if (maxSize<codeSizes[i])
			maxSize=codeSizes[i];
	for (int i=0; i<mapSize; ++i)
		chrMap[i].code=getFromBuffer(cFile,codeSizes[i]);
	ofstream uFile;
	uFile.open(toFile.c_str(),ios::out);
	while (!done)
		uFile<<getNext(cFile);
	cFile.close();
	uFile.close();
}

int Decompressor::fileSize(const string fileName)
{
	std::ifstream f;
	f.open(fileName.c_str(), std::ios_base::binary | std::ios_base::in);
	if (!f.good() || f.eof() || !f.is_open()) return 0;
	f.seekg(0, std::ios_base::beg);
	std::ifstream::pos_type begin_pos = f.tellg();
	f.seekg(0, std::ios_base::end);
	int temp = static_cast<int>(f.tellg() - begin_pos);
	f.close();
	return temp;
}

string Decompressor::getFromBuffer(ifstream& str, int leng)
{
	while ((unsigned)leng>bodyBuffer.size())
	{
		unsigned char tmp = (unsigned char)str.get();
		bodyBuffer+=charToBin(tmp);
	}
	string ret="";
	ret.reserve(leng);
	for (int i=0; i<leng; ++i)
		ret+=bodyBuffer[i];
	bodyBuffer=bodyBuffer.substr(leng);
	return ret;
}

void Decompressor::decompress(string from, string to)
{
	fromFile=from;
	toFile=to;
	extract();
}

string Decompressor::charToBin(unsigned char input)
{
	unsigned temp=(unsigned)input;
	string tmp="", tm="";
	tmp.reserve(8);
	tm.reserve(8);
	while (temp)
	{
		tmp+='0'+temp%2;
		temp/=2;
	}
	reverse(tmp.begin(), tmp.end());
	while (tmp.size()<8)
	{
		tm = "0";
		tm+=tmp;
		tmp=tm;
	}
	return tmp;
}

char Decompressor::getNext(ifstream& str)
{
	while ((unsigned)maxSize>bodyBuffer.size() && !str.eof())
	{
		unsigned char tmp = (unsigned char)str.get();
		bodyBuffer+=charToBin(tmp);
	}
    if (str.eof())
	{
		reverse(bodyBuffer.begin(),bodyBuffer.end());
		bodyBuffer=bodyBuffer.substr(endBlanks);
		reverse(bodyBuffer.begin(), bodyBuffer.end());
		done=true;
	}
	string s = "";
	s.reserve(maxSize);
	for (int i=0; i<maxSize; ++i)
	{
		s+=bodyBuffer[i];
		for (unsigned j=0; j<chrMap.size(); ++j)
			if (s==chrMap[j].code)
			{
				bodyBuffer=bodyBuffer.substr(i+1);
				char chr=chrMap[j].ch;
				return chr;
			}
	}
	return -1;
}

void Decompressor::testFile(ostream& str)
{
	ifstream file;
	file.open(fromFile.c_str(), ios::in);
	str<<"\n\nTest:\n\nMap:\n";
	for (unsigned i=0; i<chrMap.size(); ++i)
		str<<chrMap[i].ch<<": "<<chrMap[i].code<<endl;
	str<<"\nBytes:\n";
	while (!file.eof())
	{
		unsigned char chr = (unsigned char)file.get();
		string bin=charToBin(chr);
		str<<chr<<": "<<bin<<endl;
	}
	file.clear();
	file.seekg(0, ios::beg);
	str<<"\nFile contents:\n";
	while (!file.eof())
		str<<charToBin((unsigned char)file.get());
	str<<"\n\nTest ended.\n";
	file.close();
}