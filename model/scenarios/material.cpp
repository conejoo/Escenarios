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

std::string Material::toString(int scenario_index, std::string property_short_name)
{
	std::ostringstream result;
	result << "  " << id << " = type: " << type;
	for (MaterialProperty &property: properties) {
		int index = (property.short_name.compare(property_short_name) == 0) ? scenario_index : MaterialProperty::ORIGINAL_VALUE;
		result << " " << property.short_name << ": " << property.getValue(index);
	}
	if (type == 6)
		result << " name: " << strength_fn;
	return result.str();
}

std::string Material::toString()
{
	return toString(MaterialProperty::ORIGINAL_VALUE, "propiedad_inexistente");
}

int Material::getPropertyIndex(std::string short_name) {
	for (unsigned int i = 0; i < properties.size(); i++)
		if (properties[i].short_name.compare(short_name) == 0)
			return i;
	return -1;
}
