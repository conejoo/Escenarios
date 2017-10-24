#include "strengthfunction.h"
#include "materialproperty.h"
#include "utils.h"

#include <sstream>
#include <QRegExp>
#include <iostream>

StrengthFunction::StrengthFunction(std::vector<std::string> source)
{
	this->in_use = false;
	this->header = source[0];
	std::string name_needle = "name: ";
	auto pos = source[0].find(name_needle);
	this->name = source[0].substr(pos + name_needle.length());
	this->name = Utils::trim(name);
	for (unsigned int i = 1; i < source.size(); i++)
		this->values.push_back(StrengthFunctionPieSlice(Utils::getIntegers(source[i])));

	std::unordered_map<std::string, int> slices_material_ids;
	int current_material_index = 2;
	for (StrengthFunctionPieSlice &slice: values) {
		std::string material_key = "mat_";
		material_key += std::to_string(slice.cohesion);
		material_key += "___" + std::to_string(slice.phi);
		if (slices_material_ids.find(material_key) == slices_material_ids.end()) {
			current_material_index += 1;
			slices_material_ids[material_key] = current_material_index;
		}
		slice.material_index = slices_material_ids[material_key];
	}

	this->print();
}

int StrengthFunction::getPropertyIndex(std::string property_short_name) {
	if (property_short_name.compare("ang") == 0) return 0;
	if (property_short_name.compare("c") == 0) return 1;
	if (property_short_name.compare("phi") == 0) return 2;
	return -1;
}

void StrengthFunction::clearScenariosValues() {
	this->scenarios_values.clear();
}

void StrengthFunction::setScenariosValues(int scenario_index, std::vector<StrengthFunctionPieSlice> scenario_values) {
	this->scenarios_values[scenario_index] = scenario_values;
}

void StrengthFunction::print() {
	std::cout << this->name << ":" << std::endl;
	for (StrengthFunctionPieSlice& row: values)
		std::cout
				<< "\t" << row.angle
				<< "\t" << row.cohesion
				<< "\t" << row.phi
				<< "\t [MATERIAL ID: " << row.material_index << "]" << std::endl;
}

void StrengthFunction::copyPropertyBySliceMaterialIndex(std::vector<StrengthFunctionPieSlice>& target,const std::vector<StrengthFunctionPieSlice>& src, int property_index)
{
	for (StrengthFunctionPieSlice& target_slice: target) {
		for (const StrengthFunctionPieSlice& src_slice: src) {
			if (target_slice.material_index == src_slice.material_index) {
				target_slice.setValue(property_index, src_slice.getValue(property_index));
				break;
			}
		}
	}
}

std::vector<StrengthFunctionPieSlice> StrengthFunction::getMergedValues(int scenario_index, std::string property_short_name)
{
	std::vector<StrengthFunctionPieSlice> return_value;
	if (scenarios_values.find(scenario_index) != scenarios_values.end()) {
		int property_index = StrengthFunction::getPropertyIndex(property_short_name);
		if (property_index == 0) {
			// If the property was "ang", we take as base the target scenarios else the base scenario (just to keep the number of slices and materials types of the slices)
			return_value = scenarios_values[scenario_index]; // copy
			// Set phi to values from base scenario
			copyPropertyBySliceMaterialIndex(return_value, scenarios_values[MaterialProperty::ORIGINAL_VALUE], 1); // copy cohesion from base scenario
			copyPropertyBySliceMaterialIndex(return_value, scenarios_values[MaterialProperty::ORIGINAL_VALUE], 2); // copy phi from base scenario
		}
		else {
			// Get angles, phi, cohesion and number of slices from base scenario
			return_value = scenarios_values[MaterialProperty::ORIGINAL_VALUE];
			// copy the target property only (phi or cohesion)
			copyPropertyBySliceMaterialIndex(return_value, scenarios_values[scenario_index], property_index);
		}

	}
	return return_value;
}

std::string StrengthFunction::toString(int scenario_index, std::string property_short_name) {
	std::ostringstream result;
	QString fixedHeader = QString::fromStdString(header);
	if (scenarios_values.find(scenario_index) != scenarios_values.end()) {
		std::vector<StrengthFunctionPieSlice> values = getMergedValues(scenario_index, property_short_name);
		fixedHeader.replace(QRegExp("num: \\d{1,10} "), QString("num: ") + QString::number(values.size()) + QString(" "));
		result << fixedHeader.toStdString();
		for (StrengthFunctionPieSlice& value_line: values) {
			result << std::endl
				   << "    ang: " << value_line.angle
				   << " c: " << value_line.cohesion
				   << " phi: " << value_line.phi;
		}
	}
	return result.str();
}

std::string StrengthFunction::toString() {
	return this->toString(MaterialProperty::ORIGINAL_VALUE, "propiedad_inexistente");
}

