#ifndef MATERIALPROPERTY_H
#define MATERIALPROPERTY_H

#include <string>
#include <unordered_map>

class MaterialProperty
{
	public:
		MaterialProperty(std::string name, std::string value);
		std::string getValue(int n);
		static const int ORIGINAL_VALUE = -1;
		std::string name;
		std::unordered_map<int, std::string> values; // index
};

#endif // MATERIALPROPERTY_H
