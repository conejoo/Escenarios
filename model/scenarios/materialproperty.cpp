#include "materialproperty.h"
#include "utils.h"

std::unordered_map<std::string, std::wstring> MaterialProperty::translate;

MaterialProperty::MaterialProperty(std::string name, double value)
{
	short_name = name;
	std::unordered_map<std::string, std::wstring>& tr = MaterialProperty::getTranslate();
	if(tr.find(name) == tr.end()){
		this->editable = false;
		this->name = Utils::toWString(name);
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

std::unordered_map<std::string, std::wstring>& MaterialProperty::getTranslate(){
	if(MaterialProperty::translate.size() == 0){
		MaterialProperty::translate["uw"] = L"Peso Unitario (Unit Weight)";
		MaterialProperty::translate["c"] = L"Cohesión (Cohesion)";
		MaterialProperty::translate["phi"] = L"Ángulo de Fricción (Phi)";
		MaterialProperty::translate["withru"] = L"Ru (Ru)";
	}
	return translate;
}
