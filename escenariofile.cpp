#include "escenariofile.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

EscenarioFile::EscenarioFile(){}

EscenarioFile::EscenarioFile(std::string filename)
{
	std::ifstream infile(filename.c_str());
	int i = 0;
	for(std::string line; std::getline(infile, line); i++)
		lines.push_back(line);
	find_file_sections();
	process_materials();
	process_seismic();
	std::cout << "sesmic_position " << sesmic_position << std::endl;
	std::cout << "sesmicv_position " << sesmicv_position << std::endl;
	std::cout << "material_types_position " << material_types_position << std::endl;
	std::cout << "design_standard_array_position " << design_standard_array_position << std::endl;
	std::cout << "Materiales: "<< std::endl;
	for(Material m: materials)
		std::cout << m.toString() << std::endl;
}

int EscenarioFile::find_line_starting_in(int pos, const char* chars){
	std::string needle(chars);
	for(unsigned int p = pos; p < lines.size(); p++)
		if(Utils::begins_with(lines[p], needle))
			return p;
	return -1;
}

void EscenarioFile::find_file_sections()
{
	sesmic_position = find_line_starting_in(0, "  seismic:");
	sesmicv_position = find_line_starting_in(sesmic_position, "  seismicv:");
	material_types_position = find_line_starting_in(sesmic_position, "material types:");
	design_standard_array_position = find_line_starting_in(material_types_position, "design standard array:");
}

void EscenarioFile::process_materials(){
	for(int start = material_types_position + 1; start < design_standard_array_position; start++){
		std::string line = lines[start];
		if(line.length() > 4)
			materials.push_back(Material(line));
	}
}

void EscenarioFile::process_seismic(){
	int index = MaterialProperty::ORIGINAL_VALUE;
	std::string sesmic = lines[sesmic_position];
	std::string sesmicv = lines[sesmicv_position];
	std::string name;
	std::istringstream ss(sesmic);
	ss >> name;
	float value;
	ss >> value;
	sesmic_escenarios[index] = value;
	std::istringstream ssv(sesmicv);
	ssv >> name;
	ssv >> value;
	sesmicv_escenarios[index] = value;
	std::cout << "sesmicv_escenarios[index] " << sesmicv_escenarios[index] << std::endl;
}

