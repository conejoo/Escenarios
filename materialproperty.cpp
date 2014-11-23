#include "materialproperty.h"

MaterialProperty::MaterialProperty(std::string name, std::string value)
{
	this->name = name;
	int orig = ORIGINAL_VALUE;
	this->values[orig] = value;
	this->active = true;
}

std::string MaterialProperty::getValue(int n){
	if(!active){
		int index = MaterialProperty::ORIGINAL_VALUE;
		return values[index];
	}
	return values[n];
}
