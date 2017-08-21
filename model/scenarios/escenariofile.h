#ifndef ESCENARIOFILE_H
#define ESCENARIOFILE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "model/scenarios/material.h"
#include "model/scenarios/strengthfunction.h"
#include "model/scenarios/escenarioseismiccustom.h"
#include "model/scenarios/escenariomaterialcustom.h"

class EscenarioFile
{
	public:
		EscenarioFile();
		EscenarioFile(std::string filename);
		virtual ~EscenarioFile();
		EscenarioSeismicCustom* createCustomSeismicScenario(int index, std::wstring name, std::string abbr);
		EscenarioMaterialCustom* createCustomMaterialScenario(int index, std::wstring name, std::string abbr);
		void deleteSeismicScenario(int index);
		void deleteMaterialScenario(int index);
		void exportToFile(std::string filename, int seismic_index, int material_index, int property_index);
		int find_line_starting_in(int pos, const char* chars, bool throw_exception=false);
		int find_empty_line(int pos);
		void find_file_sections();
		void process_materials();
		void process_seismic();
		void process_strength_functions();
		std::vector<std::string> lines;
		std::vector<Material> materials;
		std::unordered_map<std::string, StrengthFunction*> strength_functions;
		std::unordered_map<int, EscenarioSeismicCustom*> seismic_escenarios;
		std::unordered_map<int, EscenarioMaterialCustom*> materials_escenarios;
		double base_seismic, base_seismicv;
		int sesmic_position;
		int sesmicv_position;
		int material_types_position;
		int material_types_end;
		int material_names_start;
		int strength_functions_start;
		int strength_functions_end;
		std::string filename;
};

#endif // ESCENARIOFILE_H
