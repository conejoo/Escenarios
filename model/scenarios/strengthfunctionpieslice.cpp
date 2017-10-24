#include "strengthfunctionpieslice.h"

StrengthFunctionPieSlice::StrengthFunctionPieSlice() {}
StrengthFunctionPieSlice::StrengthFunctionPieSlice(std::vector<int> values)
{
	this->angle = values[0];
	this->cohesion = values[1];
	this->phi = values[2];
}
StrengthFunctionPieSlice::StrengthFunctionPieSlice(int angle, int cohesion, int phi)
{
	this->angle = angle;
	this->cohesion = cohesion;
	this->phi = phi;
}

StrengthFunctionPieSlice::StrengthFunctionPieSlice(const StrengthFunctionPieSlice& original)
{
	angle = original.angle;
	cohesion = original.cohesion;
	phi = original.phi;
	material_index = original.material_index;
}

std::vector<int> StrengthFunctionPieSlice::getValues() const
{
	std::vector<int> values = {angle, cohesion, phi};
	return values;
}
int StrengthFunctionPieSlice::getValue(int index) const {
	if (index == 0) return angle;
	if (index == 1) return cohesion;
	if (index == 2) return phi;
	return -1;
}
int StrengthFunctionPieSlice::setValue(int index, int value) {
	if (index == 0) return angle = value;
	if (index == 1) return cohesion = value;
	if (index == 2) return phi = value;
	return -1;
}
