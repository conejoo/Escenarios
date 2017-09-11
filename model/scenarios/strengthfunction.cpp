#include "strengthfunction.h"
#include "materialproperty.h"
#include "utils.h"

#include <sstream>
#include <iostream>

StrengthFunction::StrengthFunction(std::vector<std::string> source)
{
	this->in_use = false;
	this->header = source[0];
	std::string name_needle = "name: ";
	auto pos = source[0].find(name_needle);
	this->name = source[0].substr(pos + name_needle.length());
	this->name = Utils::trim(name);
	for (unsigned int i = 1; i < source.size(); i++) {
		this->values.push_back(Utils::getIntegers(source[i]));
	}
	this->print();
}

int StrengthFunction::getPropertyIndex(std::string property_short_name) {
	if (property_short_name.compare("ang") == 0) return 0;
	if (property_short_name.compare("c") == 0) return 1;
	if (property_short_name.compare("phi") == 0) return 2;
	return -1;
}

void StrengthFunction::clearScenariosValues() {
	this->scenarios_values.clear();
}

void StrengthFunction::setScenariosValues(int scenario_index, std::vector<std::vector<int>> scenario_values) {
	this->scenarios_values[scenario_index] = scenario_values;
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

std::string StrengthFunction::toString(int scenario_index) {
	std::ostringstream result;
	result << header;
	if (scenarios_values.find(scenario_index) != scenarios_values.end()) {
		std::vector<std::vector<int>> &values = scenarios_values[scenario_index];
		for (std::vector<int>& value_line: values) {
			result << std::endl
				   << "    ang: " << value_line[0]
				   << " c: " << value_line[1]
				   << " phi: " << value_line[2];
		}
	}
	return result.str();
}

std::string StrengthFunction::toString() {
	return this->toString(MaterialProperty::ORIGINAL_VALUE);
}

