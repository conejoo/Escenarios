#include "mainwindow.h"

#include <iostream>
#include <fstream>
#include <exception>

#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>

#include "utils.h"
#include "ui_mainwindow.h"
#include "model/scenarios/escenariofile.h"
#include "gui/scenarios/materialconfigui.h"
#include "gui/scenarios/scenariosismicconfigui.h"
#include "gui/scenarios/strengthfunctionconfig.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	main_scenario(),
	scenarios_config(this),
	result_process_ui(main_scenario, this),
	general_material_config()
{
	ui->setupUi(this);
	this->showMaximized();
	connect(this->ui->actionOpen_Scenario, SIGNAL(triggered()), this, SLOT(openScenarioPromp()));
	connect(this->ui->actionExport, SIGNAL(triggered()), this, SLOT(exportScenariosPromp()));
	connect(this->ui->actionConfigurate_Scenarios, SIGNAL(triggered()), &this->scenarios_config, SLOT(show()));
	connect(&this->scenarios_config, SIGNAL(addedMaterialScenario(int,QString,QString)), this, SLOT(addMaterialScenario(int,QString,QString)));
	connect(&this->scenarios_config, SIGNAL(changedNameMaterialScenario(int,QString)), this, SLOT(changedMaterialScenarioName(int,QString)));
	connect(&this->scenarios_config, SIGNAL(changedAbbrMaterialScenario(int,QString)), this, SLOT(changedMaterialScenarioAbbr(int,QString)));
	connect(&this->scenarios_config, SIGNAL(removedMaterialScenario(int)), this, SLOT(removeMaterialScenario(int)));
	connect(&this->scenarios_config, SIGNAL(addedSismicScenario(int,QString,QString)), this, SLOT(addSismicScenario(int,QString,QString)));
	connect(&this->scenarios_config, SIGNAL(changedNameSismicScenario(int,QString)), this, SLOT(changedSismicName(int,QString)));
	connect(&this->scenarios_config, SIGNAL(changedAbbrSismicScenario(int,QString)), this, SLOT(changedSismicAbbr(int,QString)));
	connect(&this->scenarios_config, SIGNAL(removedSismicScenario(int)), this, SLOT(removeSeismicScenario(int)));
	connect(this->ui->actionProcesar_resultados, SIGNAL(triggered()), &this->result_process_ui, SLOT(show()));
	connect(&general_material_config, SIGNAL(percentajeApplied(double, int, QString)), this, SLOT(applyPercentaje(double, int, QString)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openScenarioPromp()
{
	QString filename = QFileDialog::getOpenFileName(this,
													("Abrir Escenario (.sli)"), "",
													"Scenarios (*.sli);;Todos los archivos (*)");
	if(filename.size() == 0)
		return; // cancel
	openScenario(filename.toStdString());
	result_process_ui.resetFiles();
	ui->statusBar->showMessage("Archivo cargado: " + filename, 20000);
}

void MainWindow::exportScenariosPromp()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
												"",
												QFileDialog::ShowDirsOnly
												| QFileDialog::DontResolveSymlinks);
	if(dir.size() == 0 || main_scenario.materials.size() == 0)
		return; // cancel
	int n = 0;
	std::wofstream _parametros;
	std::string _par_filename = dir.toStdString() + "/" + main_scenario.filename + "_parametros.csv";
	_parametros.open(_par_filename.c_str());
	_parametros << "Escenario" << ",";
	_parametros << "CCS Horizontal" << ",";
	_parametros << "CCS Vertical" << ",";
	_parametros << "Material" << ",";
	{
		for (Material& material: main_scenario.materials) {
			if (material.type == 0) {
				std::vector<MaterialProperty>& props = main_scenario.materials[0].properties;
				for(MaterialProperty& prop: props)
					if(prop.editable)
						_parametros << prop.name << ",";
				break;
			}
		}
	}
	_parametros << std::endl;
	for (StrengthFunctionConfig *config: strength_functions_ui)
		config->readScenariosValues();
	for (const auto& ite: main_scenario.materials_escenarios) {
		EscenarioMaterialCustom* material_es = ite.second;
		if(!material_es->enabled)
			continue;
		QString material_scenario_abbr = QString::fromStdString(material_es->abbr);
		for (const auto& ite2: main_scenario.seismic_escenarios) {
			EscenarioSeismicCustom* seismic_es = ite2.second;
			if (!seismic_es->enabled)
				continue;
			QString seismic_scenario_abbr = QString::fromStdString(seismic_es->abbr);
			QString filename = dir + "/" +
				   QString::fromStdString(main_scenario.filename) + "_" +
				   seismic_scenario_abbr + "_" +
				   material_scenario_abbr;
			if (material_es->index != MaterialProperty::ORIGINAL_VALUE) {
				for (auto props: qcheckbox_property_index) {
					MaterialProperty* property = props.second;
					if (!property->active || !property->editable || property->short_name.compare("ang") == 0) // No generar archivos para ANG!!
						continue;
					std::cout << "Propiedad para exportar: " << property->short_name << std::endl;
					QString complete_filename = filename + "_" + QString::fromStdString(property->short_name) + ".sli";
					main_scenario.exportToFile(complete_filename.toStdString(), seismic_es->index, material_es->index, property->short_name);
					std::cout << "Exported to: " << complete_filename.toStdString() << std::endl;
					n++;
					printFileParametersLine(_parametros, seismic_es, property->short_name, material_es->index,
											complete_filename.right(complete_filename.length()-dir.length()-1));
				}
			}
			else {
				filename = filename + ".sli";
				main_scenario.exportToFile(filename.toStdString(), seismic_es->index, material_es->index, "propiedad_inexistente");
				std::cout << "Exported to: " << filename.toStdString() << std::endl;
				n++;
				printFileParametersLine(_parametros, seismic_es, "propiedad_inexistente", -1,
										filename.right(filename.length()-dir.length()-1));
			}
		}
	}
	_parametros.close();
	QMessageBox msgBox;
	msgBox.setText(QString::number(n) +
				   " nuevos escenarios han sido guardados en la carpeta " +
				   dir);
	msgBox.exec();
}

void MainWindow::printFileParametersLine(std::wofstream& file, EscenarioSeismicCustom *seismic_es, std::string property_short_name, int scenario_index, QString complete_filename) {
	for (Material& material: main_scenario.materials) {
		file << complete_filename.toStdWString() << ",";
		file << seismic_es->seismic << ",";
		file << seismic_es->seismicv << ",";
		file << Utils::toWString(material.name) << ",";
		for (MaterialProperty &property: material.properties) {
			if (!property.editable)
				continue;
			int p_index = MaterialProperty::ORIGINAL_VALUE;
			if (property.short_name.compare(property_short_name) == 0)
				p_index = scenario_index;
			file << property.getValue(p_index) << ",";
		}
		file << std::endl;
	}
}

void MainWindow::openScenario(std::string filename) {
	clearLayout(ui->widget_seismic->layout());
	clearLayout(ui->widget_sensibilizar->layout());
	clearLayout(ui->widget_materials->layout());
	ui->generalPropertiesContainer->layout()->takeAt(0); //Save general material config
	clearLayout(ui->materialsTab->layout());
	clearLayout(ui->strengthFunctionsTab->layout()); // Strength functions
	materials_ui.clear();
	strength_functions_ui.clear();
	custom_seismic_schenarios_ui.clear();
	qcheckbox_property_index.clear();
	qcheckbox_material_scenario_index.clear();
	index_qcheckbox_material_scenario.clear();
	general_material_config.clearScenarios();
	try {
		main_scenario = EscenarioFile(filename);
		addProperties();
		addMaterials();
		addStrengthFunctions();
		scenarios_config.setScenarioFile(&main_scenario);
		result_process_ui.setEscenarioFile(main_scenario);
	}
	catch (std::exception& e) {
		std::cout << "Standard exception: " << e.what() << std::endl;
		QMessageBox messageBox;
		messageBox.critical(
			0,
			"Error trying to load SLI file",
			QString::fromStdString(
				std::string("There is a problem with the format of the file.\n") +
				std::string(e.what())
			)
		);
		messageBox.setFixedSize(500, 200);
	}
}

void MainWindow::clearLayout(QLayout *layout) {
	QLayoutItem *item;
	while ((item = layout->takeAt(0))) {
		if (item->layout()) {
			clearLayout(item->layout());
			delete item->layout();
		}
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}
}

void MainWindow::addMaterialScenario(int index, QString name, QString abbr) {
	EscenarioMaterialCustom* new_scenario = main_scenario.createCustomMaterialScenario(index, name.toStdWString(), abbr.toStdString());
	for (MaterialConfigUI* config: materials_ui)
		config->escenarioAdded(index, name);
	for (StrengthFunctionConfig* config: strength_functions_ui)
		config->escenarioAdded(index, name);
	QCheckBox* qcheckbox = new QCheckBox(name + " (" + abbr + ")", ui->groupBox_escenarios);
	qcheckbox->setChecked(true);
	if (index == MaterialProperty::ORIGINAL_VALUE)
		qcheckbox->setDisabled(true);
	ui->widget_materials->layout()->addWidget(qcheckbox);
	for (Material &material: main_scenario.materials) {
		if (material.type == 0) {
			general_material_config.addScenario(new_scenario, material, main_scenario.strength_functions.size() > 0);
			break;
		}
	}
	index_qcheckbox_material_scenario[index] = qcheckbox;
	qcheckbox_material_scenario_index[qcheckbox] = index;
	connect(qcheckbox, SIGNAL(toggled(bool)), this, SLOT(toggleMaterialScenario(bool)));
}

void MainWindow::addSismicScenario(int index, QString name, QString abbr){
	EscenarioSeismicCustom* custom = main_scenario.createCustomSeismicScenario(index, name.toStdWString(), abbr.toStdString());
	main_scenario.seismic_escenarios[index] = custom;
	ScenarioSismicConfigUI* config = new ScenarioSismicConfigUI(this, custom);
	config->setNewName(name, abbr);
	custom_seismic_schenarios_ui[index] = config;
	ui->widget_seismic->layout()->addWidget(config);
}

void MainWindow::changedMaterialScenarioName(int index, QString new_name) {
	EscenarioMaterialCustom* custom = main_scenario.materials_escenarios[index];
	for (MaterialConfigUI* config: materials_ui)
		config->escenarioChangedName(index, new_name);
	for (StrengthFunctionConfig* config: strength_functions_ui)
		config->escenarioChangedName(index, new_name);
	QCheckBox* checkbox = index_qcheckbox_material_scenario[index];
	checkbox->setText(new_name + " (" + QString::fromStdString(custom->abbr) + ")");
	custom->name = new_name.toStdWString();
	general_material_config.escenarioChangedName(index, new_name);
}

void MainWindow::changedMaterialScenarioAbbr(int index, QString abbr) {
	EscenarioMaterialCustom* custom = main_scenario.materials_escenarios[index];
	QCheckBox* checkbox = index_qcheckbox_material_scenario[index];
	checkbox->setText(QString::fromStdWString(custom->name) + " ("+abbr+")");
	custom->abbr = abbr.toStdString();
}

void MainWindow::removeMaterialScenario(int index) {
	for (MaterialConfigUI* config: materials_ui)
		config->escenarioRemoved(index);
	for (StrengthFunctionConfig *config: strength_functions_ui)
		config->escenarioRemoved(index);
	QCheckBox* checkbox = index_qcheckbox_material_scenario[index];
	index_qcheckbox_material_scenario.erase(index);
	qcheckbox_material_scenario_index.erase(checkbox);
	ui->widget_materials->layout()->removeWidget(checkbox);
	delete checkbox;
	main_scenario.deleteMaterialScenario(index);
	general_material_config.escenarioRemoved(index);
}

void MainWindow::removeSeismicScenario(int index){
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	ui->widget_seismic->layout()->removeWidget(config);
	delete config;
	main_scenario.deleteSeismicScenario(index);
}

void MainWindow::changedSismicName(int index, QString new_name) {
	EscenarioSeismicCustom* custom = main_scenario.seismic_escenarios[index];
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	config->setNewName(new_name, QString::fromStdString(custom->abbr));
}

void MainWindow::changedSismicAbbr(int index, QString abbr) {
	EscenarioSeismicCustom* custom = main_scenario.seismic_escenarios[index];
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	config->setNewName(QString::fromStdWString(custom->name), abbr);
}

void MainWindow::addMaterials() {
	ui->generalPropertiesContainer->layout()->addWidget(&general_material_config);
	for(Material& material: main_scenario.materials){
		MaterialConfigUI* config = new MaterialConfigUI(this, material);
		materials_ui.push_back(config);
		ui->materialsTab->layout()->addWidget(config);
	}
}

void MainWindow::addStrengthFunctions() {
	for (StrengthFunction* function: main_scenario.strength_functions) {
		StrengthFunctionConfig* config = new StrengthFunctionConfig(this, function);
		strength_functions_ui.push_back(config);
		ui->strengthFunctionsTab->layout()->addWidget(config);
	}
}

void MainWindow::addProperties(){
	if (main_scenario.materials.size() == 0)
		return;
	// Add properties from material without strength function
	for (Material &material: main_scenario.materials) {
		if (material.type == 0) {
			for (MaterialProperty &property: main_scenario.materials[0].properties) {
				if(!property.editable)
					continue;
				QCheckBox* qcheckbox = new QCheckBox(QString::fromStdWString(property.name), ui->widget_sensibilizar);
				qcheckbox->setChecked(true);
				ui->widget_sensibilizar->layout()->addWidget(qcheckbox);
				qcheckbox_property_index[qcheckbox] = &property;
				connect(qcheckbox, SIGNAL(toggled(bool)), this, SLOT(toggleProperty(bool)));
			}
			break;
		}
	}
	if (main_scenario.strength_functions.size() != 0) {
		MaterialProperty *ang_property = new MaterialProperty("ang", 0.0);
		QCheckBox* qcheckbox = new QCheckBox(QString::fromStdWString(ang_property->name), ui->widget_sensibilizar);
		qcheckbox->setChecked(true);
		ui->widget_sensibilizar->layout()->addWidget(qcheckbox);
		qcheckbox_property_index[qcheckbox] = ang_property;
		connect(qcheckbox, SIGNAL(toggled(bool)), this, SLOT(toggleProperty(bool)));
	}
}

void MainWindow::toggleProperty(bool toggled){
	QCheckBox* checkbox = qobject_cast<QCheckBox *>(sender());
	MaterialProperty* property = qcheckbox_property_index[checkbox];
	if (property) {
		QString property_short_name = QString::fromStdString(property->short_name);
		property->active = toggled;
		for (MaterialConfigUI* material_ui: materials_ui)
			material_ui->toggleProperty(property_short_name, toggled);
		for (StrengthFunctionConfig *strength_function_ui: strength_functions_ui)
			strength_function_ui->toggleProperty(property_short_name, toggled);
		general_material_config.toggleProperty(property_short_name, toggled);
	}
}

void MainWindow::toggleMaterialScenario(bool toggled){
	QCheckBox* checkbox = qobject_cast<QCheckBox *>(sender());
	if (qcheckbox_material_scenario_index.find(checkbox) != qcheckbox_material_scenario_index.end()) {
		int index = qcheckbox_material_scenario_index[checkbox];
		if (index) {
			for (MaterialConfigUI* material_ui: materials_ui)
				material_ui->toggleScenario(index, toggled);
			for (StrengthFunctionConfig* strength_function_ui: strength_functions_ui)
				strength_function_ui->toggleScenario(index, toggled);
			main_scenario.materials_escenarios[index]->enabled = toggled;
			general_material_config.toggleMaterial(index, toggled);
		}
	}
}

void MainWindow::applyPercentaje(double percent, int scenario_index, QString property_short_name) {
	std::cout << "Apply percentaje "<< percent << " " << scenario_index << " " << property_short_name.toStdString() << std::endl;
	for (MaterialConfigUI* material_ui: materials_ui)
		material_ui->applyPercentaje(percent, scenario_index, property_short_name);
	for (StrengthFunctionConfig *strength_function_ui: strength_functions_ui)
		strength_function_ui->applyPercentaje(percent, scenario_index, property_short_name);
}
