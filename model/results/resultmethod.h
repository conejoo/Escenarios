#ifndef RESULTMETHOD_H
#define RESULTMETHOD_H

#include <unordered_map>
#include <utility>

class ResultMethod
{
	public:
		ResultMethod(std::string name);
		std::string name;
		std::pair<std::string, std::string> getValue(std::string);
		std::unordered_map<std::string, std::pair<std::string, std::string> > values;
};

#endif // RESULTMETHOD_H
