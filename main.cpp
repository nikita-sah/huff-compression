#include <iostream>
#include "counter.h"
#include "HuffmanCoding.h"
#include "Compressor.h"
#include "Decompressor.h"
#include <gtkmm.h>
#include <string>
using namespace Gtk;
using namespace std;

int inspectFile(string);
void createMap();

CountingSet cs;
vector<double> pers;
vector<char> chrs;
vector<string> codes;

struct comparison
{
	bool operator() (Pair& i, Pair& j) 
	{ 
		return (i.code.length()<j.code.length());
	}
} comparer;

vector<Pair> mapper;




class GTKTest : public Window
{
	// controls

	protected: 
		VBox vBoxMain;
		Label labelTextToDisplay;
		Entry entryTextToDisplay;
		Button btnCompress;
		Button btnDecompress;

	// methods

	public: GTKTest()
	{
		set_border_width(10);
		set_title("project");
		// vBoxMain
		add(vBoxMain);
		vBoxMain.show();

		// labelTextToDisplay
		labelTextToDisplay.set_label("Please select Compress/Decompress");
		vBoxMain.add(labelTextToDisplay);
		labelTextToDisplay.show();

		// entryTextToDisplay
		entryTextToDisplay.set_text("Type File name here");
		vBoxMain.add(entryTextToDisplay);
		entryTextToDisplay.show();


		// buttonDisplay
		btnCompress.set_label("Compress");
		btnCompress.signal_clicked().connect
		(
			sigc::mem_fun
			(
				*this,
				&GTKTest::btnCompressClicked
			)
		);
		vBoxMain.add(btnCompress);
		btnCompress.show();

		btnDecompress.set_label("Decompress");
		btnDecompress.signal_clicked().connect
		(
			sigc::mem_fun
			(
				*this,
				&GTKTest::btnDecompressClicked
			)
		);
		vBoxMain.add(btnDecompress);
		btnDecompress.show();
	}

	// event handlers

	protected: void btnCompressClicked()
	{
		string fileName = entryTextToDisplay.get_text();

		if (!inspectFile(fileName))
		{
			setStatus("File Not found!");
			return;
		}
		cs.getPercentages(pers);
		cs.getCharacters(chrs);
		HuffmanCoding hCodes(chrs,pers);
		hCodes.getCodes(codes, chrs);
		createMap();
		Compressor comp(mapper);
		comp.compress(fileName, fileName+".huff");
		setStatus("Compression done");	
	}

	protected: void btnDecompressClicked()
	{
		string fileName = entryTextToDisplay.get_text();
		std::size_t pos = fileName.find(".huff");
		if( pos == -1){
			setStatus("Unknows Compresion format");
			return;
		}
		string decomprName = fileName.substr(0,pos);
		Decompressor decomp;
		decomp.decompress(fileName,decomprName);
		setStatus("Decompression done");

	}
	void setStatus(string status){
		labelTextToDisplay.set_label(status);

	}

	int inspectFile(string name)
	{
		ifstream dat;
		dat.open(name.c_str(), ios::in);
		if (!dat.is_open())
			return 0;
		while (!dat.eof())
			cs.addElement(dat.get());
		dat.close();
		cs.removeLast();
		return 1;
	}

void createMap(){
	for (unsigned i=0; i<chrs.size(); ++i)
	{
		Pair temp;
		temp.ch=chrs[i];
		temp.code=codes[i];
		mapper.push_back(temp);
	}
	sort(mapper.begin(), mapper.end(), comparer);
	}

};

int main (int argc, char *argv[])
{
	Main kit(argc, argv);
	GTKTest GTKTest;
	Main::run(GTKTest);
	return 0;
}
