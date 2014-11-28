#include "materialproperty.h"

std::unordered_map<std::string, std::string> MaterialProperty::translate;

MaterialProperty::MaterialProperty(std::string name, double value)
{
	short_name = name;
	std::unordered_map<std::string, std::string>& tr = MaterialProperty::getTranslate();
	if(tr.find(name) == tr.end()){
		this->editable = false;
		this->name = name;
	}else{
		this->editable = true;
		this->name = tr[name];
	}
	int orig = ORIGINAL_VALUE;
	this->values[orig] = value;
	this->active = true;
}

double MaterialProperty::getValue(int n){
	if(!active){
		int index = MaterialProperty::ORIGINAL_VALUE;
		return values[index];
	}
	return values[n];
}

std::unordered_map<std::string, std::string>& MaterialProperty::getTranslate(){
	if(MaterialProperty::translate.size() == 0){
		MaterialProperty::translate["uw"] = "Peso Unitario (Unit Weight)";
		MaterialProperty::translate["c"] = "Cohesión (Cohesion)";
		MaterialProperty::translate["phi"] = "Ángulo de Fricción (Phi)";
		MaterialProperty::translate["withru"] = "Ru (Ru)";
	}
	return translate;
}
