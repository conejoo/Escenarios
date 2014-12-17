#ifndef MATERIALPROPERTY_H
#define MATERIALPROPERTY_H

#include <string>
#include <unordered_map>

class MaterialProperty
{
	public:
		MaterialProperty(std::string name, double value);
		double getValue(int n);
		static const int ORIGINAL_VALUE = -1;
		std::wstring name;
		std::string short_name;
		bool active;
		bool editable;
		std::unordered_map<int, double> values; // index
		static std::unordered_map<std::string, std::wstring> translate;
		static std::unordered_map<std::string, std::wstring>& getTranslate();
};

#endif // MATERIALPROPERTY_H