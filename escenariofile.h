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
		void exportToFile(std::string filename, int seismic_index, int material_index);
		int find_line_starting_in(int pos, const char* chars);
		int find_empty_line(int pos);
		void find_file_sections();
		void process_materials();
		void process_seismic();
		std::vector<std::string> lines;
		std::vector<Material> materials;
		std::unordered_map<int, std::string> sesmic_escenarios;
		std::unordered_map<int, std::string> sesmicv_escenarios;
		std::unordered_map<int, std::string> seismic_scenarios_ids;
		std::unordered_map<int, std::string> materials_scenarios_ids;
		int sesmic_position;
		int sesmicv_position;
		int material_types_position;
		int material_types_end;
};

#endif // ESCENARIOFILE_H
