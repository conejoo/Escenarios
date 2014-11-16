#include "materialproperty.h"

MaterialProperty::MaterialProperty(std::string name, float value)
{
	this->name = name;
	int orig = ORIGINAL_VALUE;
	this->values[orig] = value;
}

float MaterialProperty::getValue(int n){
	return values[n];
}
