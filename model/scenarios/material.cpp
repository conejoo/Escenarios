#include "material.h"

#include "utils.h"

#include <sstream>
#include <iostream>


Material::Material(std::string material_string, std::string material_description)
{
	// Get description
	strength_fn = "";
	std::istringstream ssdes(material_description);
	std::string token;
	ssdes >> token;
	name = "";
	std::cout << "material_string" << material_string;
	std::cout << "material_description" << material_description;
	while(token.compare("red:")){
		name = name + token;
		ssdes >> token;
	}
	std::string tmp;
	std::istringstream ss(material_string);
	ss >> id;
	ss >> tmp; // discard =
	while (ss >> tmp) {
		if (tmp == "name:") {
			std::stringstream second;
			second << ss.rdbuf();
			strength_fn = second.str();
			strength_fn = Utils::trim(strength_fn);
			std::cout << "Material " << name << " with str_function: " << strength_fn << std::endl;
			break;
		}
		else if (tmp == "type:") {
			int value;
			ss >> value;
			type = value;
		}
		else {
			double value;
			ss >> value;
			properties.push_back(MaterialProperty(tmp.substr(0,tmp.length()-1), value));
		}
	}
}

std::string Material::toString(int n, int property_index)
{
	std::ostringstream result;
	result << "  " << id << " =";
	for(int i = 0; i < (int)properties.size(); i++){
		MaterialProperty &property = properties[i];
		int index = (i != property_index) ? MaterialProperty::ORIGINAL_VALUE : n;
		result << " " << property.short_name << ": " << property.getValue(index);
	}
	return result.str();
}

std::string Material::toString()
{
	return toString(MaterialProperty::ORIGINAL_VALUE, 0);
}

int Material::getPropertyIndex(std::string short_name) {
	for (unsigned int i = 0; i < properties.size(); i++)
		if (properties[i].short_name.compare(short_name) == 0)
			return i;
	return -1;
}
