#include "resultfile.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

ResultFile::ResultFile(std::string file):
	filename(file.substr(file.find_last_of("\\/")+1,file.length()-file.find_last_of("\\/")-1))
{
	included = false;
	std::ifstream infile(file.c_str());
	for(std::string line; std::getline(infile, line);)
		lines.push_back(line);
	processFileData();
	processFileName();
}


int ResultFile::findLineStartingIn(const char* chars){
	return findLineStartingIn(0, chars);
}


int ResultFile::findLineStartingIn(int pos, const char* chars){
	std::string needle(chars);
	for(unsigned int p = pos; p < lines.size(); p++)
		if(Utils::begins_with(lines[p], needle))
			return p;
	return -1;
}

void ResultFile::processFileName(){
	std::string filename_no_ext = Utils::removeFileExtension(filename);
	std::vector<std::string> file_split = Utils::split(filename_no_ext, '_');
	if(file_split.size() < 3){
		std::cout << "No se pudo procesar nombre de archivo: " << filename << std::endl;
		return;
	}
	sensible = file_split[file_split.size()-1];
	file_split.pop_back();
	material_scenario = file_split[file_split.size()-1];
	file_split.pop_back();
	seismic_scenario = file_split[file_split.size()-1];
	file_split.pop_back();
	std::cout << "Si se pudo procesar nombre de archivo: " << filename << " Sensible: " << sensible<< std::endl;
}

void ResultFile::processFileData(){
	global_minimum_fs_pos = findLineStartingIn(0, "* Global Minimum FS");
	global_minimum_text_pos = findLineStartingIn(global_minimum_fs_pos, "* Global Minimum Text");
	int nmethods = global_minimum_text_pos-global_minimum_fs_pos-1;
	std::cout << "* Global Minimum FS: " << global_minimum_fs_pos << std::endl;
	std::cout << "* Global Minimum Text: " << global_minimum_text_pos << std::endl;
	std::cout << "Cantidad de metodos: " << nmethods << std::endl;
	for(int i = 0; i < nmethods; i++){
		std::string token;
		std::stringstream stream(lines[global_minimum_fs_pos + 1 + i]);
		for(int j = 0; j < 8; j++)
			stream >> token;
		std::string fs = token;
		std::string name = "";
		while(stream >> token)
			name += token + " ";
		name = Utils::trim(name);
		ResultMethod method(name);
		method.values["FS"] = std::pair<std::string, std::string>(fs,"");
		methods.push_back(method);
	}
	int start = global_minimum_text_pos + 1;
	for(ResultMethod& method: methods){
		int n = 0;
		std::stringstream stream(lines[start]);
		stream >> n;
		std::cout << "Metodo: " << method.name << ", N propiedades: " << n << std::endl;
		start++;
		for(int i = 0; i < n; i++){
			std::string current_line = lines[start];
			int pos = current_line.find_first_of("=");
			std::string property_name = current_line.substr(0, pos);
			std::string property_value_unit = current_line.substr(pos+1, current_line.length()-pos-1);
			std::cout << "Property name: " << property_name << std::endl;
			std::cout << "Property value: " << property_value_unit << std::endl;
			std::stringstream value_unit_stream(property_value_unit);
			std::string value;
			std::string unit;
			value_unit_stream >> value;
			value_unit_stream >> unit;
			method.values[property_name] = std::pair<std::string, std::string>(value,unit);
			start++;
		}
	}
}

