#include "escenariofile.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

EscenarioFile::EscenarioFile(){}

EscenarioFile::EscenarioFile(std::string filename):
	filename(filename.substr(filename.find_last_of("\\/")+1,filename.find_last_of(".")-filename.find_last_of("\\/")-1))
{
	std::ifstream infile(filename.c_str());
	for(std::string line; std::getline(infile, line);)
		lines.push_back(line);
	find_file_sections();
	process_materials();
	process_seismic();
	std::cout << "sesmic_position " << sesmic_position << std::endl;
	std::cout << "sesmicv_position " << sesmicv_position << std::endl;
	std::cout << "material_types_position " << material_types_position << std::endl;
	std::cout << "material_types_end_position " << material_types_end << std::endl;
	std::cout << "Materiales: "<< std::endl;
	for(Material m: materials)
		std::cout << m.toString() << std::endl;
}
EscenarioFile::~EscenarioFile()
{
	for(auto& it: seismic_escenarios)
		delete it.second;
	for(auto& it: materials_escenarios)
		delete it.second;
}


int EscenarioFile::find_empty_line(int pos){
	for(unsigned int p = pos; p < lines.size(); p++)
        if(Utils::trim(lines[p]).length() == 0)
			return p;
	return -1;
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
	material_types_end = find_empty_line(material_types_position);
	material_names_start= find_line_starting_in(material_types_end, "material properties:");
}

void EscenarioFile::process_materials(){
	for(int start = material_types_position + 1; start < material_types_end; start++){
		std::string line = lines[start];
		if(line.length() > 4){
			std::string description = lines[start-material_types_position + material_names_start];
			materials.push_back(Material(line, description));
		}
	}
}

void EscenarioFile::process_seismic(){
	std::string sesmic = lines[sesmic_position];
	std::string sesmicv = lines[sesmicv_position];
	std::string name;
	std::istringstream ss(sesmic);
	ss >> name;
	double value;
	ss >> value;
	base_seismic = value;
	std::istringstream ssv(sesmicv);
	ssv >> name;
	ssv >> value;
	base_seismicv = value;
	std::cout << "base_seismic " << base_seismic << std::endl;
	std::cout << "base_seismicv " << base_seismicv << std::endl;
}
EscenarioSeismicCustom* EscenarioFile::createCustomSeismicScenario(int index, std::string name, std::string abbr){
	EscenarioSeismicCustom* new_custom = new EscenarioSeismicCustom(index, name, abbr);
	if(index == MaterialProperty::ORIGINAL_VALUE){
		new_custom->seismic = base_seismic;
		new_custom->seismicv = base_seismicv;
	}
	seismic_escenarios[index] = new_custom;
	return new_custom;
}
EscenarioMaterialCustom* EscenarioFile::createCustomMaterialScenario(int index, std::string name, std::string abbr){
	EscenarioMaterialCustom* new_custom = new EscenarioMaterialCustom(index, name, abbr);
	materials_escenarios[index] = new_custom;
	return new_custom;
}

void EscenarioFile::deleteSeismicScenario(int index){
	EscenarioSeismicCustom* custom = seismic_escenarios[index];
	seismic_escenarios.erase(index);
	delete custom;
}

void EscenarioFile::deleteMaterialScenario(int index){
	EscenarioMaterialCustom* custom = materials_escenarios[index];
	materials_escenarios.erase(index);
	delete custom;
}

void EscenarioFile::exportToFile(std::string filename, int seismic_index, int material_index, int property_index){
	std::cout << "Exporting to file: " << filename << std::endl;
	std::ofstream myfile;
	myfile.open(filename.c_str());
	for(int i = 0; i < sesmic_position; i++)
		myfile << lines[i] << std::endl;
	myfile << "  seismic: " << seismic_escenarios[seismic_index]->seismic << std::endl;
	for(int i = sesmic_position + 1; i < sesmicv_position; i++)
		myfile << lines[i] << std::endl;
	myfile << "  seismicv: " << seismic_escenarios[seismic_index]->seismicv << std::endl;
	for(int i = sesmicv_position + 1; i <= material_types_position; i++)
		myfile << lines[i] << std::endl;
	for(Material &m: materials)
		myfile << m.toString(material_index, property_index) << std::endl;
	for(int i = material_types_end; i < (int)lines.size(); i++)
		myfile << lines[i] << std::endl;
	myfile.close();
}
