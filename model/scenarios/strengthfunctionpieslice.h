#ifndef STRENGTHFUNCTIONPIESLICE_H
#define STRENGTHFUNCTIONPIESLICE_H

#include <vector>

class StrengthFunctionPieSlice
{
	public:
		StrengthFunctionPieSlice();
		StrengthFunctionPieSlice(std::vector<int> values);
		StrengthFunctionPieSlice(int angle, int cohesion, int phi);
		StrengthFunctionPieSlice(const StrengthFunctionPieSlice&);

		std::vector<int> getValues() const;
		int getValue(int index) const;
		int setValue(int index, int value);
		int angle;
		int phi;
		int cohesion;
		// Las rebanadas del pie representan un material, el material normalmente esta repetido simetricamente
		int material_index;
};

#endif // STRENGTHFUNCTIONPIESLICE_H
