#include "resultsprocessui.h"
#include "ui_resultsprocessui.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <fstream>
#include <locale>

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
	translation_scenario["min"] = L"Escenario Mínimo";
	translation_scenario["med"] = L"Escenario Medio";
	translation_scenario["max"] = L"Escenario Máximo";
	translation_scenario["base"] = L"Base";
	translation_scenario["op"] = L"Sismo Operacional";
	translation_scenario["ab"] = L"Abandono";
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
	QLayoutItem* item = 0;
	while((item = ui->widget_archivos->layout()->takeAt(0))){
		delete item->widget();
		delete item;
	}
	ui->label_resume->setText(QString::number(results.size()) + " Archivos cargados...");
}

void ResultsProcessUI::loadFilesPromp(){
	QStringList filenames = QFileDialog::getOpenFileNames(this,
													("Open File"), "",
													"Results (*.s01 *.sl1)");
	if(filenames.size() == 0)
		return; // cancel
	for(QString filename: filenames){
		ui->widget_archivos->layout()->addWidget(new QLabel(filename));
		results.push_back(new ResultFile(filename.toStdString()));
	}
	ui->label_resume->setText(QString::number(results.size()) + " Archivos cargados...");
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
