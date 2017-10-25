#include "resultsprocessui.h"
#include "ui_resultsprocessui.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <fstream>
#include <locale>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "utils.h"
#include "model/results/resultfile.h"

// Implementation of the NORMDIST excel function
// https://stackoverflow.com/questions/4934217/magic-numbers-in-c-implementation-of-excel-normdist-function
static double normdist(double y, double mean, double standard_dev)
{
    double res;
    double x = (y - mean) / standard_dev;
    if (x == 0)
    {
        res=0.5;
    }
    else
    {
        double oor2pi = 1/(sqrt(double(2) * 3.14159265358979323846));
        double t = 1 / (double(1) + 0.2316419 * fabs(x));
        t *= oor2pi * exp(-0.5 * x * x)
             * (0.31938153   + t
             * (-0.356563782 + t
             * (1.781477937  + t
             * (-1.821255978 + t * 1.330274429))));
        if (x >= 0)
        {
            res = double(1) - t;
        }
        else
        {
            res = t;
        }
    }
    return res;
}

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

    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", "CSV File (*.csv);;All files (*.*)");
    if(filename.size() == 0)
		return; // cancel

    std::string std_filename = filename.toStdString();
    std::string std_summary_filename = std_filename.substr(0, std_filename.find(".")) + "_summary.csv";
    std::wofstream myfile, summaryFile;
	myfile.open(std_filename.c_str());
    summaryFile.open(std_summary_filename.c_str());

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

    std::map<std::string, std::map<std::string, std::map<std::string, double>>> summary;

	for(ResultFile* rs: results){

		for(ResultMethod& method: rs->methods){
			myfile << Utils::toWString(rs->filename) << ",";
			myfile << Utils::toWString(method.name) << ",";

            // This will be used to fill the summary CSV file
            // Only take into account information about the gle method!
            if (method.name.find("gle") != std::string::npos){
                if(isMedMaterialScenario(rs->sensible))
                {
                    summary[rs->material_scenario]["-"][rs->sensible] = std::stod(method.getValue("FS").first);
                }
                else
                {
                    summary[rs->seismic_scenario][rs->sensible][rs->material_scenario] = std::stod(method.getValue("FS").first);
                }
            }

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


    summaryFile << L"Casos,";
    summaryFile << L"FS base,";
    summaryFile << L"Probabilidad de Falla,";
    summaryFile << L"Media,";
    summaryFile << L"Desviación Estándar,";
    summaryFile << L"C FS min,";
    summaryFile << L"C FS max,";
    summaryFile << L"C FS delta,";
    summaryFile << L"Phi FS min,";
    summaryFile << L"Phi FS max,";
    summaryFile << L"Phi FS delta,";
    summaryFile << L"Ang FS min,";
    summaryFile << L"Ang FS max,";
    summaryFile << L"Ang FS delta,";
    summaryFile << std::endl;

    // Filling the summary CSV file
    for (auto const& sismic_index : summary)
    {
        auto sensible_index = sismic_index.second;
        sensible_index["c"]["delta"] = sensible_index["c"]["max"] - sensible_index["c"]["min"];
        sensible_index["phi"]["delta"] = sensible_index["phi"]["max"] - sensible_index["phi"]["min"];
		sensible_index["ang"]["delta"] = sensible_index["ang"]["max"] - sensible_index["ang"]["min"];
		auto seismic_avg = (sensible_index["c"]["min"] + sensible_index["c"]["max"]
				+ sensible_index["phi"]["min"] + sensible_index["phi"]["max"]
				+ sensible_index["ang"]["min"] + sensible_index["ang"]["max"]) / 6.0;
		auto seismic_sd = sqrt(pow(sensible_index["c"]["delta"] / 2.0, 2)
				+ pow(sensible_index["phi"]["delta"] / 2.0, 2)
				+ pow(sensible_index["ang"]["delta"] / 2.0, 2));

        summaryFile << translateSeismicScenario(sismic_index.first) << ",";
        summaryFile << sensible_index["-"]["med"] << ",";

        // probabilidad de falla (excel =NORMDIST(1,E5,F5,TRUE))
        summaryFile << normdist(1.0, seismic_avg, seismic_sd) << ",";

        // promedio
        summaryFile << seismic_avg << ",";

        // desviación estándar
        summaryFile << seismic_sd << ",";

        summaryFile << sensible_index["c"]["min"] << ",";
        summaryFile << sensible_index["c"]["max"] << ",";
        summaryFile << sensible_index["c"]["delta"] << ",";

        summaryFile << sensible_index["phi"]["min"] << ",";
        summaryFile << sensible_index["phi"]["max"] << ",";
        summaryFile << sensible_index["phi"]["delta"] << ",";

		summaryFile << sensible_index["ang"]["min"] << ",";
		summaryFile << sensible_index["ang"]["max"] << ",";
		summaryFile << sensible_index["ang"]["delta"];

        summaryFile << std::endl;
    }
    summaryFile.close();
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
