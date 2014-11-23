#include "resultfile.h"
#include "utils.h"
#include <fstream>
#include <iostream>

ResultFile::ResultFile(std::string file):
	filename(file)
{
	std::ifstream infile(file.c_str());
	for(std::string line; std::getline(infile, line);)
		lines.push_back(line);
	processFileData();
}


int ResultFile::findLineStartingIn(const char* chars){
	return findLineStartingIn(0, chars);
}


int ResultFile::findLineStartingIn(int pos, const char* chars){
	std::string needle(chars);
	for(unsigned int p = pos; p < lines.size(); p++)
		if(Utils::begins_with(lines[p], needle))
			return p;
	return -1;
}

void ResultFile::processFileData(){
	int global_minimum_pos = findLineStartingIn(0, "* Global Minimum FS");
	std::cout << "* Global Minimum FS: " << global_minimum_pos << std::endl;
}
