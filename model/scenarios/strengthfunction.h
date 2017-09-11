#ifndef STRENGTHFUNCTION_H
#define STRENGTHFUNCTION_H

#include <vector>
#include <string>
#include <unordered_map>

class StrengthFunction
{
	public:
		StrengthFunction(std::vector<std::string> source);
		void clearScenariosValues();
		void setScenariosValues(int scenario_index, std::vector<std::vector<int>> scenario_values);
		std::string toString(int scenario_index);
		std::string toString();
		void print();

		static int getPropertyIndex(std::string property_short_name);

		std::string header;
		std::string name;
		std::vector<std::vector<int>> values;
		std::unordered_map<int, std::vector<std::vector<int>>> scenarios_values;
		bool in_use;
};

#endif // STRENGTHFUNCTION_H
