#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include "materialproperty.h"

class StrengthFunction;

class Material
{
	public:
		Material(std::string material_string, std::string material_description);
		int getPropertyIndex(std::string short_name);
		std::string toString();
		std::string toString(int n, int property_index); // print using n-prop
		std::string id;
		std::string name;
		int type;
		std::vector<MaterialProperty> properties;
		std::string strength_fn;
};

#endif // MATERIAL_H
