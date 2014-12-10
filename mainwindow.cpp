#include "mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <QCheckBox>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "escenariofile.h"
#include "materialconfigui.h"
#include "scenariosismicconfigui.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	main_scenario(),
	scenarios_config(this),
	result_process_ui(main_scenario, this)
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
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openScenarioPromp(){
	QString filename = QFileDialog::getOpenFileName(this,
													("Abrir Escenario (.sli)"), "",
													"Scenarios (*.sli);;Todos los archivos (*)");
	if(filename.size() == 0)
		return; // cancel
	openScenario(filename.toStdString());
	result_process_ui.resetFiles();
	ui->statusBar->showMessage("Archivo cargado: " + filename, 20000);
}

void MainWindow::exportScenariosPromp(){
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
												"",
												QFileDialog::ShowDirsOnly
												| QFileDialog::DontResolveSymlinks);
	if(dir.size() == 0 || main_scenario.materials.size() == 0)
		return; // cancel
	int n = 0;
	for (const auto& ite: main_scenario.materials_escenarios) {
		EscenarioMaterialCustom* material_es = ite.second;
		if(!material_es->enabled)
			continue;
		QString material_scenario_abbr = QString::fromStdString(material_es->abbr);
		for(const auto& ite2: main_scenario.seismic_escenarios){
			EscenarioSeismicCustom* seismic_es = ite2.second;
			if(!seismic_es->enabled)
				continue;
			QString seismic_scenario_abbr = QString::fromStdString(seismic_es->abbr);
			QString filename = dir + "/" +
				   QString::fromStdString(main_scenario.filename) + "_" +
				   seismic_scenario_abbr + "_" +
				   material_scenario_abbr;
			if(material_es->index != MaterialProperty::ORIGINAL_VALUE){
				Material& material = main_scenario.materials[0];
				for(int p = 0; p < (int)material.properties.size(); p++){
					if(!material.properties[p].active || !material.properties[p].editable)
						continue;
					QString complete_filename = filename + "_" + QString::fromStdString(material.properties[p].short_name) + ".sli";
					main_scenario.exportToFile(complete_filename.toStdString(), seismic_es->index, material_es->index, p);
					std::cout << "Exported to: " << complete_filename.toStdString() << std::endl;
					n++;
				}
			}else{
				filename = filename + ".sli";
				main_scenario.exportToFile(filename.toStdString(), seismic_es->index, material_es->index, -1);
				std::cout << "Exported to: " << filename.toStdString() << std::endl;
				n++;
			}
		}
	}
	QMessageBox msgBox;
	msgBox.setText(QString::number(n) +
				   " nuevos escenarios han sido guardados en la carpeta " +
				   dir);
	msgBox.exec();
}

void MainWindow::openScenario(std::string filename){
	clearLayout(ui->widget_seismic->layout());
	clearLayout(ui->widget_sensibilizar->layout());
	clearLayout(ui->widget_materials->layout());
	clearLayout(ui->scrollAreaWidgetContents->layout());
	materials_ui.clear();
	custom_seismic_schenarios_ui.clear();
	qcheckbox_property_index.clear();
	qcheckbox_material_scenario_index.clear();
	index_qcheckbox_material_scenario.clear();
	main_scenario = EscenarioFile(filename);
	addProperties();
	addMaterials();
	scenarios_config.setScenarioFile(&main_scenario);
	result_process_ui.setEscenarioFile(main_scenario);
}

void MainWindow::clearLayout(QLayout *layout){
	QLayoutItem *item;
	while((item = layout->takeAt(0))) {
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

void MainWindow::addMaterialScenario(int index, QString name, QString abbr){
	main_scenario.createCustomMaterialScenario(index, name.toStdWString(), abbr.toStdString());
	for(MaterialConfigUI* config: materials_ui)
		config->escenarioAdded(index, name);
	QCheckBox* qcheckbox = new QCheckBox(name + " (" + abbr + ")", ui->groupBox_escenarios);
	qcheckbox->setChecked(true);
	if(index == MaterialProperty::ORIGINAL_VALUE)
		qcheckbox->setDisabled(true);
	ui->widget_materials->layout()->addWidget(qcheckbox);
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

void MainWindow::changedMaterialScenarioName(int index, QString new_name){
	EscenarioMaterialCustom* custom = main_scenario.materials_escenarios[index];
	for(MaterialConfigUI* config: materials_ui)
		config->escenarioChangedName(index, new_name);
	QCheckBox* checkbox = index_qcheckbox_material_scenario[index];
	checkbox->setText(new_name + " (" + QString::fromStdString(custom->abbr) + ")");
	custom->name = new_name.toStdWString();
}

void MainWindow::changedMaterialScenarioAbbr(int index, QString abbr){
	EscenarioMaterialCustom* custom = main_scenario.materials_escenarios[index];
	QCheckBox* checkbox = index_qcheckbox_material_scenario[index];
	checkbox->setText(QString::fromStdWString(custom->name) + " ("+abbr+")");
	custom->abbr = abbr.toStdString();
}

void MainWindow::removeMaterialScenario(int index){
	for(MaterialConfigUI* config: materials_ui)
		config->escenarioRemoved(index);
	QCheckBox* checkbox = index_qcheckbox_material_scenario[index];
	index_qcheckbox_material_scenario.erase(index);
	qcheckbox_material_scenario_index.erase(checkbox);
	ui->widget_materials->layout()->removeWidget(checkbox);
	delete checkbox;
	main_scenario.deleteMaterialScenario(index);
}

void MainWindow::removeSeismicScenario(int index){
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	ui->widget_seismic->layout()->removeWidget(config);
	delete config;
	main_scenario.deleteSeismicScenario(index);
}

void MainWindow::changedSismicName(int index, QString new_name){
	EscenarioSeismicCustom* custom = main_scenario.seismic_escenarios[index];
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	config->setNewName(new_name, QString::fromStdString(custom->abbr));
}

void MainWindow::changedSismicAbbr(int index, QString abbr){
	EscenarioSeismicCustom* custom = main_scenario.seismic_escenarios[index];
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	config->setNewName(QString::fromStdWString(custom->name), abbr);
}

void MainWindow::addMaterials(){
	for(Material& material: main_scenario.materials){
		MaterialConfigUI* config = new MaterialConfigUI(this,material);
		materials_ui.push_back(config);
		ui->scrollAreaWidgetContents->layout()->addWidget(config);
	}
}

void MainWindow::addProperties(){
	if(main_scenario.materials.size() == 0)
		return;
	for(MaterialProperty &property: main_scenario.materials[0].properties){
		if(!property.editable)
			continue;
		QCheckBox* qcheckbox = new QCheckBox(QString::fromStdString(property.name), ui->widget_sensibilizar);
		qcheckbox->setChecked(true);
		ui->widget_sensibilizar->layout()->addWidget(qcheckbox);
		qcheckbox_property_index[qcheckbox] = &property;
		connect(qcheckbox, SIGNAL(toggled(bool)), this, SLOT(toggleProperty(bool)));
	}
}

void MainWindow::toggleProperty(bool toggled){
	QCheckBox* checkbox = qobject_cast<QCheckBox *>(sender());
	MaterialProperty* property = qcheckbox_property_index[checkbox];
	if(property){
		property->active = toggled;
	}
	QWidget* parent = (QWidget*) checkbox->parent();
	int index = parent->layout()->indexOf(checkbox);
	for(MaterialConfigUI* material_ui: materials_ui){
		material_ui->toggleProperty(index, toggled);
	}
}

void MainWindow::toggleMaterialScenario(bool toggled){
	QCheckBox* checkbox = qobject_cast<QCheckBox *>(sender());
	int index = qcheckbox_material_scenario_index[checkbox];
	if(index){
		for(MaterialConfigUI* material_ui: materials_ui)
			material_ui->toggleMaterial(index, toggled);
		main_scenario.materials_escenarios[index]->enabled = toggled;
	}
}
