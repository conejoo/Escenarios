#ifndef MATERIALPROPERTY_H
#define MATERIALPROPERTY_H

#include <string>
#include <unordered_map>

class MaterialProperty
{
	public:
		MaterialProperty(std::string name, double value);
		double getValue(int scenario_index);
		static int ORIGINAL_VALUE;
		std::wstring name;
		std::string short_name;
		bool active;
		bool editable;
		std::unordered_map<int, double> values; // values on each scenario
		static std::unordered_map<std::string, std::wstring> translate;
		static std::unordered_map<std::string, std::wstring>& getTranslate();
};

#endif // MATERIALPROPERTY_H
