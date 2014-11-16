#include "material.h"
#include <sstream>

Material::Material(std::string material_string)
{
	std::string tmp;
	std::istringstream ss(material_string);
	ss >> name;
	ss >> tmp; // discard =
	while(ss >> tmp){
		float value;
		ss >> value;
		properties.push_back(MaterialProperty(tmp, value));
	}
}

std::string Material::toString(int n)
{
	std::ostringstream result;
	result << "  " << name << " =";
	for(MaterialProperty property: properties)
		result << " " << property.name << " " << property.getValue(n);
	return result.str();
}

std::string Material::toString()
{
	return toString(MaterialProperty::ORIGINAL_VALUE);
}
