#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include "materialproperty.h"

class Material
{
	public:
		Material(std::string material_string, std::string material_description);
		std::string toString();
		std::string toString(int n, int property_index); // print using n-prop
		std::string id;
		std::string name;
		std::vector<MaterialProperty> properties;
};

#endif // MATERIAL_H
