//MIT License
//Copyright 2017 Patrick Laughrea

#include <iostream>
#include <fstream>

#include "notesToText/parser.hpp"
#include "various/smartIterator.hpp"

using namespace ntt;
using namespace std;
using namespace various;

int main(int argc, char** argv)
{
	SmartIterator it("");
	if (argc <= 1)
		it = SmartIterator(cin.rdbuf());
	else if (argc > 2)
	{
		cerr << "Usage: " << argv[0] << " [file]" << endl;
		return 1;
	}
	else
	{
		ifstream fileIn(argv[1], ios::binary);
		if (fileIn.fail())
		{
			cerr << "Error: failed to open file \"" << argv[1] << "\"" << endl;
			return 1;
		}
		it = SmartIterator(fileIn.rdbuf());
	}
	try
	{
		auto lines = Parser::parse(move(it));
		for (const auto& line : lines)
			cout << line << endl;
	}
	catch (const exception& e)
	{
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}