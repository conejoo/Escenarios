#ifndef RESULTSFILE_H
#define RESULTSFILE_H

#include <string>
#include <vector>

class ResultFile
{
	public:
		ResultFile(std::string file);
		std::string filename;
		std::vector<std::string> lines;
	private:
		void processFileData();
		int findLineStartingIn(const char* chars);
		int findLineStartingIn(int pos, const char* chars);
};

#endif // RESULTSFILE_H
