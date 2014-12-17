#include "resultsprocessui.h"
#include "ui_resultsprocessui.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <fstream>
#include <locale>
#include <iostream>

#include "utils.h"
#include "model/results/resultfile.h"

ResultsProcessUI::ResultsProcessUI(EscenarioFile& es, QWidget *parent) :
	QWidget(parent, Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint),
	ui(new Ui::ResultsProcessUI),
	escenarios(es),
	custom_file_dialog(this)
{
	ui->setupUi(this);
	connect(ui->pushButton_export, SIGNAL(clicked()), this, SLOT(exportResultsPromp()));
	connect(ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(resetFiles()));
	connect(ui->pushButton_load, SIGNAL(clicked()), this, SLOT(loadFilesPromp()));
	connect(&custom_file_dialog, SIGNAL(newFileSelection(QStringList)), this, SLOT(openProcessFiles(QStringList)));
	translation_scenario["min"] = L"Escenario Mínimo";
	translation_scenario["med"] = L"Escenario Medio";
	translation_scenario["max"] = L"Escenario Máximo";
	translation_scenario["base"] = L"Estático";
	translation_scenario["op"] = L"Sismo Operacional";
	translation_scenario["ab"] = L"Abandono";
	countAddedFiles();
}

ResultsProcessUI::~ResultsProcessUI()
{
	delete ui;
}

void ResultsProcessUI::setEscenarioFile(EscenarioFile& new_esc){
	escenarios = new_esc;
}

void ResultsProcessUI::resetFiles(){
	for(ResultFile* result: results)
		delete result;
	results.clear();
	ui->listWidget_not_added->clear();
	results_map.clear();
	ui->label_resume->setText(QString::number(results.size()) + " Archivos cargados...");
	countAddedFiles();
}
void ResultsProcessUI::openProcessFiles(const QStringList & filenames){
	if(filenames.size() == 0)
		return; // cancel
	for(QString filename: filenames){
		if(!filename.endsWith(".s01", Qt::CaseInsensitive)){
			std::cout << "Archivo descartado: " << filename.toStdString() << std::endl;
			continue;
		}
		if(results_map.find(filename.toStdString()) != results_map.end())
			continue;
		ui->listWidget_not_added->addItem(filename);
		ResultFile* new_file = new ResultFile(filename.toStdString());
		results.push_back(new_file);
		results_map[filename.toStdString()] = new_file;
	}
	ui->label_resume->setText(QString::number(results.size()) + " Archivos cargados...");
	countAddedFiles();
}

void ResultsProcessUI::loadFilesPromp(){
	custom_file_dialog.openDialog();
	return;
	QStringList filenames = QFileDialog::getOpenFileNames(this,
													("Abrir resultados (.s01)"), "",
													"Resultados (*.s01);;Todos los archivos (*)");

}

void ResultsProcessUI::addResultFile(){
//	QList<QListWidgetItem*> selected_list = ui->listWidget_not_added->selectedItems();
//	for(QListWidgetItem* item: selected_list){
//		QString filename = item->text();
//		std::cout << "Added to list: " << filename.toStdString() << std::endl;
//		ui->listWidget_not_added->removeItemWidget(item);
//		ui->listWidget_added->addItem(filename);
//		results_map[filename.toStdString()]->included = true;
//		delete item;
//	}
//	if(ui->listWidget_not_added->count() > 0)
//		ui->listWidget_not_added->item(0)->setSelected(true);
//	countAddedFiles();
}

void ResultsProcessUI::removeResultFile(){
//	QList<QListWidgetItem*> selected_list = ui->listWidget_added->selectedItems();
//	for(QListWidgetItem* item: selected_list){
//		QString filename = item->text();
//		std::cout << "Removed from list: " << filename.toStdString() << std::endl;
//		ui->listWidget_added->removeItemWidget(item);
//		ui->listWidget_not_added->addItem(filename);
//		results_map[filename.toStdString()]->included = false;
//		delete item;
//	}
//	if(ui->listWidget_added->count() > 0)
//		ui->listWidget_added->item(0)->setSelected(true);
//	countAddedFiles();
}

void ResultsProcessUI::addAllResultFiles(){
//	ui->listWidget_not_added->selectAll();
//	this->addResultFile();
}

void ResultsProcessUI::removeAllResultFiles(){
//	ui->listWidget_added->selectAll();
//	this->removeResultFile();
}

void ResultsProcessUI::exportResultsPromp(){
	if(results.size() == 0){
		QMessageBox msgBox;
		msgBox.setText("Para exportar los resultados primero necesita cargar por lo menos 1 archivo.");
		msgBox.exec();
		return;
	}
	QString filename = QFileDialog::getSaveFileName(this, "Save file", "",
											   "CSV File (*.csv);;All files (*.*)");
	if(filename.size() == 0)
		return; // cancel
	std::string std_filename = filename.toStdString();
	std::wofstream myfile;
	myfile.open(std_filename.c_str());
	//header
	myfile << L"Archivo,Método,";
	myfile << L"Escenario Sísmico,";
	myfile << L"Variable sensible,";
	myfile << L"Escenario sensibilidad,";
	myfile << L"FS,";
	myfile << Utils::toWString(addUnit("Resisting Moment")) << ",";
	myfile << Utils::toWString(addUnit("Driving Moment")) << ",";
	myfile << Utils::toWString(addUnit("Resisting Horizontal Force")) << ",";
	myfile << Utils::toWString(addUnit("Driving Horizontal Force")) << ",";
	myfile << Utils::toWString(addUnit("Total Slice Area")) << std::endl;
	for(ResultFile* rs: results){
//		if(!rs->included)
//			continue;
		for(ResultMethod& method: rs->methods){
			myfile << Utils::toWString(rs->filename) << ",";
			myfile << Utils::toWString(method.name) << ",";
			if(isMedMaterialScenario(rs->sensible)){
				if(areTranslatables(rs->material_scenario, rs->sensible)){
					myfile << translateSeismicScenario(rs->material_scenario) << ",";
					myfile << "" << ",";
					myfile << translateMaterialScenario(rs->sensible) << ",";
				}else{
					myfile  << ",,,";
				}
			}else{
				if(areTranslatables(rs->seismic_scenario, rs->material_scenario)){
					myfile << translateSeismicScenario(rs->seismic_scenario) << ",";
					myfile << Utils::toWString(rs->sensible) << ",";
					myfile << translateMaterialScenario(rs->material_scenario) << ",";
				}else{
					myfile  << ",,,";
				}
			}
			myfile << Utils::toWString(method.getValue("FS").first) << ",";
			myfile << Utils::toWString(method.getValue("Resisting Moment").first) << ",";
			myfile << Utils::toWString(method.getValue("Driving Moment").first) << ",";
			myfile << Utils::toWString(method.getValue("Resisting Horizontal Force").first) << ",";
			myfile << Utils::toWString(method.getValue("Driving Horizontal Force").first) << ",";
			myfile << Utils::toWString(method.getValue("Total Slice Area").first);
			myfile << std::endl;
		}
	}
	myfile.close();
	QMessageBox msgBox;
	msgBox.setText("CSV exportado con éxito a " + filename);
	msgBox.exec();
}
std::string ResultsProcessUI::addUnit(const char* name){
	std::string sname(name);
	for(ResultFile* rs: results){
		for(ResultMethod& method: rs->methods){
			if(method.values.find(sname) != method.values.end())
				return sname + " (" + method.values[sname].second + ")";
		}
	}
	return sname;
}

std::wstring ResultsProcessUI::translateMaterialScenario(std::string abbr){
	for(const auto& it: escenarios.materials_escenarios){
		EscenarioMaterialCustom* material_es = it.second;
		if(material_es->abbr.compare(abbr) == 0)
			return material_es->name;
	}
	return localTranslate(abbr);
}

bool ResultsProcessUI::isMedMaterialScenario(std::string abbr){
	for(const auto& it: escenarios.materials_escenarios){
		EscenarioMaterialCustom* material_es = it.second;
		if((material_es->abbr.compare(abbr) == 0 && material_es->index == MaterialProperty::ORIGINAL_VALUE)){
			return true;
		}
	}
	return abbr.compare("med") == 0;
}

bool ResultsProcessUI::areTranslatables(std::string abbr_se, std::string abbr_mat){
	bool seis_tras = translation_scenario.find(abbr_se) != translation_scenario.end();
	bool mat_tras = translation_scenario.find(abbr_mat) != translation_scenario.end();
	if(seis_tras && mat_tras)
		return true;
	for(const auto& it: escenarios.materials_escenarios){
		EscenarioMaterialCustom* material_es = it.second;
		if((material_es->abbr.compare(abbr_mat) == 0) ||
				( translation_scenario.find(abbr_mat) != translation_scenario.end())){
			mat_tras = true;
			break;
		}
	}
	for(const auto& it: escenarios.seismic_escenarios){
		EscenarioSeismicCustom* material_es = it.second;
		if((material_es->abbr.compare(abbr_se) == 0) ||
				( translation_scenario.find(abbr_se) != translation_scenario.end())){
			seis_tras = true;
			break;
		}
	}
	return seis_tras && mat_tras;
}

std::wstring ResultsProcessUI::translateSeismicScenario(std::string abbr){
	for(const auto& it: escenarios.seismic_escenarios){
		EscenarioSeismicCustom* seismic_es = it.second;
		if(seismic_es->abbr.compare(abbr) == 0)
			return seismic_es->name;
	}
	return localTranslate(abbr);
}

std::wstring ResultsProcessUI::localTranslate(std::string abbr){
	if(translation_scenario.find(abbr) == translation_scenario.end()){
		std::wstring wabbr(abbr.begin(), abbr.end());
		return wabbr;
	}
	return translation_scenario[abbr];
}

void ResultsProcessUI::countAddedFiles(){
//	int not_added = ui->listWidget_not_added->count();
//	int added = ui->listWidget_added->count();
//	ui->label_added_files->setText(QString::number(added) + " Archivos agregados");
//	ui->label_not_added_files->setText(QString::number(not_added) + " Archivos no agregados");
}
