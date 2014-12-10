#include "resultsprocessui.h"
#include "ui_resultsprocessui.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <fstream>
#include <locale>
#include <iostream>

#include "resultfile.h"

ResultsProcessUI::ResultsProcessUI(EscenarioFile& es, QWidget *parent) :
	QWidget(parent, Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint),
	ui(new Ui::ResultsProcessUI),
	escenarios(es)
{
	ui->setupUi(this);
	connect(ui->pushButton_export, SIGNAL(clicked()), this, SLOT(exportResultsPromp()));
	connect(ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(resetFiles()));
	connect(ui->pushButton_load, SIGNAL(clicked()), this, SLOT(loadFilesPromp()));
	connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(addResultFile()));
	connect(ui->pushButton_remove, SIGNAL(clicked()), this, SLOT(removeResultFile()));
	connect(ui->pushButton_add_all, SIGNAL(clicked()), this, SLOT(addAllResultFiles()));
	connect(ui->pushButton_remove_all, SIGNAL(clicked()), this, SLOT(removeAllResultFiles()));
	translation_scenario["min"] = L"Escenario Mínimo";
	translation_scenario["med"] = L"Escenario Medio";
	translation_scenario["max"] = L"Escenario Máximo";
	translation_scenario["base"] = L"Base";
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
	ui->listWidget_added->clear();
	ui->listWidget_not_added->clear();
	results_map.clear();
	ui->label_resume->setText(QString::number(results.size()) + " Archivos cargados...");
	countAddedFiles();
}

void ResultsProcessUI::loadFilesPromp(){
	QStringList filenames = QFileDialog::getOpenFileNames(this,
													("Abrir resultados (.s01)"), "",
													"Resultados (*.s01);;Todos los archivos (*)");
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

void ResultsProcessUI::addResultFile(){
	QList<QListWidgetItem*> selected_list = ui->listWidget_not_added->selectedItems();
	for(QListWidgetItem* item: selected_list){
		QString filename = item->text();
		std::cout << "Added to list: " << filename.toStdString() << std::endl;
		ui->listWidget_not_added->removeItemWidget(item);
		ui->listWidget_added->addItem(filename);
		results_map[filename.toStdString()]->included = true;
		delete item;
	}
	if(ui->listWidget_not_added->count() > 0)
		ui->listWidget_not_added->item(0)->setSelected(true);
	countAddedFiles();
}

void ResultsProcessUI::removeResultFile(){
	QList<QListWidgetItem*> selected_list = ui->listWidget_added->selectedItems();
	for(QListWidgetItem* item: selected_list){
		QString filename = item->text();
		std::cout << "Removed from list: " << filename.toStdString() << std::endl;
		ui->listWidget_added->removeItemWidget(item);
		ui->listWidget_not_added->addItem(filename);
		results_map[filename.toStdString()]->included = false;
		delete item;
	}
	if(ui->listWidget_added->count() > 0)
		ui->listWidget_added->item(0)->setSelected(true);
	countAddedFiles();
}

void ResultsProcessUI::addAllResultFiles(){
	ui->listWidget_not_added->selectAll();
	this->addResultFile();
}

void ResultsProcessUI::removeAllResultFiles(){
	ui->listWidget_added->selectAll();
	this->removeResultFile();
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
	myfile << toWString(addUnit("Resisting Moment")) << ",";
	myfile << toWString(addUnit("Driving Moment")) << ",";
	myfile << toWString(addUnit("Resisting Horizontal Force")) << ",";
	myfile << toWString(addUnit("Driving Horizontal Force")) << ",";
	myfile << toWString(addUnit("Total Slice Area")) << std::endl;
	for(ResultFile* rs: results){
		if(!rs->included)
			continue;
		for(ResultMethod& method: rs->methods){
			myfile << toWString(rs->filename) << ",";
			myfile << toWString(method.name) << ",";
			myfile << translateSeismicScenario(rs->seismic_scenario) << ",";
			myfile << toWString(rs->sensible) << ",";
			myfile << translateMaterialScenario(rs->material_scenario) << ",";
			myfile << toWString(method.getValue("FS").first) << ",";
			myfile << toWString(method.getValue("Resisting Moment").first) << ",";
			myfile << toWString(method.getValue("Driving Moment").first) << ",";
			myfile << toWString(method.getValue("Resisting Horizontal Force").first) << ",";
			myfile << toWString(method.getValue("Driving Horizontal Force").first) << ",";
			myfile << toWString(method.getValue("Total Slice Area").first);
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

std::wstring ResultsProcessUI::toWString(std::string s){
	return QString::fromStdString(s).toStdWString();
}

void ResultsProcessUI::countAddedFiles(){
	int not_added = ui->listWidget_not_added->count();
	int added = ui->listWidget_added->count();
	ui->label_added_files->setText(QString::number(added) + " Archivos agregados");
	ui->label_not_added_files->setText(QString::number(not_added) + " Archivos no agregados");
}
