#include "resultmethod.h"

ResultMethod::ResultMethod(std::string name):
	name(name)
{
}

std::pair<std::string, std::string> ResultMethod::getValue(std::string property){
	if(values.find(property) == values.end())
		return std::pair<std::string, std::string>("-","");
	return values[property];
}
