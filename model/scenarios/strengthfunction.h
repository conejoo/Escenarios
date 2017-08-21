#ifndef STRENGTHFUNCTION_H
#define STRENGTHFUNCTION_H

#include <vector>
#include <string>

class StrengthFunction
{
	public:
		StrengthFunction(std::vector<std::string> source);
		void print();
		std::string name;
		std::vector<std::vector<int>> values;
		bool in_use;
};

#endif // STRENGTHFUNCTION_H
