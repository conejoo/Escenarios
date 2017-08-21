#include "strengthfunction.h"
#include "utils.h"

#include <sstream>
#include <iostream>

StrengthFunction::StrengthFunction(std::vector<std::string> source)
{
	in_use = false;
	std::string name_needle = "name: ";
	auto pos = source[0].find(name_needle);
	name = source[0].substr(pos + name_needle.length());
	name = Utils::trim(name);
	for (unsigned int i = 1; i < source.size(); i++) {
		values.push_back(Utils::getIntegers(source[i]));
	}
	this->print();
}

void StrengthFunction::print() {
	std::cout << this->name << ":" << std::endl;
	for (auto row: values) {
		std::cout << "\t";
		for (auto col: row) {
			std::cout << col << "\t";
		}
		std::cout << std::endl;
	}
}
