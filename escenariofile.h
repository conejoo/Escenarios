#ifndef ESCENARIOFILE_H
#define ESCENARIOFILE_H

#include <string>
#include <vector>
#include <material.h>
#include <unordered_map>
#include <unordered_set>

class EscenarioFile
{
	public:
		EscenarioFile();
		EscenarioFile(std::string filename);
		int find_line_starting_in(int pos, const char* chars);
		void find_file_sections();
		void process_materials();
		void process_seismic();
		std::vector<std::string> lines;
		std::vector<Material> materials;
		std::unordered_map<int, float> sesmic_escenarios;
		std::unordered_map<int, float> sesmicv_escenarios;
		std::unordered_set<int> seismic_scenarios_ids;
		std::unordered_set<int> materials_scenarios_ids;
		int sesmic_position;
		int sesmicv_position;
		int material_types_position;
		int design_standard_array_position;
};

#endif // ESCENARIOFILE_H
