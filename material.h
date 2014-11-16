#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include "materialproperty.h"

class Material
{
	public:
		Material(std::string material_string);
		std::string toString();
		std::string toString(int n); // print using n-prop
		std::string name;
		std::vector<MaterialProperty> properties;
};

#endif // MATERIAL_H
