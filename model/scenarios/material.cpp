#include "material.h"
#include <sstream>

Material::Material(std::string material_string, std::string material_description)
{
	// Get description
	std::istringstream ssdes(material_description);
	std::string token;
	ssdes >> token;
	name = "";
	while(token.compare("red:")){
		name = name + token;
		ssdes >> token;
	}
	std::string tmp;
	std::istringstream ss(material_string);
	ss >> id;
	ss >> tmp; // discard =
	while(ss >> tmp){
		double value;
		ss >> value;
		properties.push_back(MaterialProperty(tmp.substr(0,tmp.length()-1), value));
	}
}

std::string Material::toString(int n, int property_index)
{
	std::ostringstream result;
	result << "  " << id << " =";
	for(int i = 0; i < (int)properties.size(); i++){
		MaterialProperty &property = properties[i];
		int index = (i != property_index)?MaterialProperty::ORIGINAL_VALUE:n;
		result << " " << property.short_name << ": " << property.getValue(index);
	}
	return result.str();
}

std::string Material::toString()
{
	return toString(MaterialProperty::ORIGINAL_VALUE, 0);
}
