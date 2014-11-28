#ifndef RESULTSFILE_H
#define RESULTSFILE_H

#include <string>
#include <vector>
#include "resultmethod.h"

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
		int global_minimum_fs_pos;
		int global_minimum_text_pos;
		std::vector<ResultMethod> methods;
};

#endif // RESULTSFILE_H
