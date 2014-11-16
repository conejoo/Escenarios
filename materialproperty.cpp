#include "materialproperty.h"

MaterialProperty::MaterialProperty(std::string name, std::string value)
{
	this->name = name;
	int orig = ORIGINAL_VALUE;
	this->values[orig] = value;
}

std::string MaterialProperty::getValue(int n){
	return values[n];
}
