#ifndef STRENGTHFUNCTION_H
#define STRENGTHFUNCTION_H

#include <vector>
#include <string>
#include <unordered_map>
#include "model/scenarios/strengthfunctionpieslice.h"

class StrengthFunction
{
	public:
		StrengthFunction(std::vector<std::string> source);
		void clearScenariosValues();
		void setScenariosValues(int scenario_index, std::vector<StrengthFunctionPieSlice> scenario_values);
		std::string toString(int scenario_index, std::string property_short_name);
		std::string toString();
		void print();

		static int getPropertyIndex(std::string property_short_name);

		std::string header;
		std::string name;
		std::vector<StrengthFunctionPieSlice> values;
		std::unordered_map<int, std::vector<StrengthFunctionPieSlice>> scenarios_values;

		bool in_use;

	private:
		void copyPropertyBySliceMaterialIndex(std::vector<StrengthFunctionPieSlice>& target,const std::vector<StrengthFunctionPieSlice>& src, int property_index);
		std::vector<StrengthFunctionPieSlice> getMergedValues(int scenario_index, std::string property_short_name);
};

#endif // STRENGTHFUNCTION_H
