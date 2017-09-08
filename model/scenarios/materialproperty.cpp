#include "materialproperty.h"
#include "utils.h"

std::unordered_map<std::string, std::wstring> MaterialProperty::translate;
int MaterialProperty::ORIGINAL_VALUE = -1;

MaterialProperty::MaterialProperty(std::string name, double value)
{
	short_name = name;
	std::unordered_map<std::string, std::wstring>& tr = MaterialProperty::getTranslate();
	if (tr.find(name) == tr.end()) {
		this->editable = false;
		this->name = Utils::toWString(name);
	}
	else {
		this->editable = true;
		this->name = tr[name];
	}
	this->values[MaterialProperty::ORIGINAL_VALUE] = value;
	this->active = true;
}

double MaterialProperty::getValue(int scenario_index){
	if (!active)
		return values[MaterialProperty::ORIGINAL_VALUE];
	return values[scenario_index];
}

std::unordered_map<std::string, std::wstring>& MaterialProperty::getTranslate(){
	if (MaterialProperty::translate.size() == 0) {
		MaterialProperty::translate["uw"] = L"Peso Unitario (Unit Weight)";
		MaterialProperty::translate["c"] = L"Cohesión (Cohesion)";
		MaterialProperty::translate["phi"] = L"Ángulo de Fricción (Phi)";
		MaterialProperty::translate["ang"] = L"Ángulo Strength Function (ang)";
		//MaterialProperty::translate["withru"] = L"Ru (Ru)";
	}
	return translate;
}
