#include "mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <QCheckBox>
#include "ui_mainwindow.h"
#include "escenariofile.h"
#include "materialconfigui.h"
#include "scenariosismicconfigui.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	scenarios_config(this),
	result_process_ui(this)
{
	ui->setupUi(this);
	this->showMaximized();
	connect(this->ui->actionOpen_Scenario, SIGNAL(triggered()), this, SLOT(openScenarioPromp()));
	connect(this->ui->actionExport, SIGNAL(triggered()), this, SLOT(exportScenariosPromp()));
	connect(this->ui->actionConfigurate_Scenarios, SIGNAL(triggered()), &this->scenarios_config, SLOT(show()));
	connect(&this->scenarios_config, SIGNAL(addedMaterialScenario(int,QString)), this, SLOT(addMaterialScenario(int,QString)));
	connect(&this->scenarios_config, SIGNAL(changedNameMaterialScenario(int,QString)), this, SLOT(changedMaterialScenarioName(int,QString)));
	connect(&this->scenarios_config, SIGNAL(removedMaterialScenario(int)), this, SLOT(removeMaterialScenario(int)));
	connect(&this->scenarios_config, SIGNAL(addedSismicScenario(int,QString)), this, SLOT(addSismicScenario(int,QString)));
	connect(&this->scenarios_config, SIGNAL(changedNameSismicScenario(int,QString)), this, SLOT(changedSismicName(int,QString)));
	connect(&this->scenarios_config, SIGNAL(removedSismicScenario(int)), this, SLOT(removeSeismicScenario(int)));
	connect(this->ui->actionProcesar_resultados, SIGNAL(triggered()), &this->result_process_ui, SLOT(show()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openScenarioPromp(){
	QString filename = QFileDialog::getOpenFileName(this,
													("Open File"), "",
													"Scenarios (*.sli)");
	if(filename.size() == 0)
		return; // cancel
	openScenario(filename.toStdString());
}

void MainWindow::exportScenariosPromp(){
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
												"",
												QFileDialog::ShowDirsOnly
												| QFileDialog::DontResolveSymlinks);
	if(dir.size() == 0)
		return; // cancel
	for (const auto& ite: main_scenario.materials_scenarios_ids) {
		int material_id = ite.first;
		if(!index_qcheckbox_material_scenario[material_id]->isChecked())
			continue;
		QString material_scenario_name = QString::fromStdString(ite.second);
		for(const auto& ite2: main_scenario.seismic_scenarios_ids){
			int seismic_id = ite2.first;
			if(!custom_seismic_schenarios_ui[seismic_id]->isChecked())
				continue;
			QString seismic_scenario_name = QString::fromStdString(ite2.second);
			QString filename = dir + "/" + material_scenario_name + "_" + seismic_scenario_name + ".sli";
			main_scenario.exportToFile(filename.toStdString(), seismic_id, material_id);
		}
	}
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

void MainWindow::addMaterialScenario(int index, QString name){
	for(MaterialConfigUI* config: materials_ui)
		config->escenarioAdded(index, name);
	QCheckBox* qcheckbox = new QCheckBox(name, ui->groupBox_escenarios);
	qcheckbox->setChecked(true);
	ui->widget_materials->layout()->addWidget(qcheckbox);
	index_qcheckbox_material_scenario[index] = qcheckbox;
	qcheckbox_material_scenario_index[qcheckbox] = index;
	connect(qcheckbox, SIGNAL(toggled(bool)), this, SLOT(toggleMaterialScenario(bool)));
	main_scenario.materials_scenarios_ids[index] = name.toStdString();
}

void MainWindow::addSismicScenario(int index, QString name){
	ScenarioSismicConfigUI* config = new ScenarioSismicConfigUI(this, main_scenario, index);
	config->setNewName(name);
	custom_seismic_schenarios_ui[index] = config;
	ui->widget_seismic->layout()->addWidget(config);
	main_scenario.seismic_scenarios_ids[index] = name.toStdString();
}

void MainWindow::changedMaterialScenarioName(int index, QString new_name){
	for(MaterialConfigUI* config: materials_ui)
		config->escenarioChangedName(index, new_name);
	QCheckBox* checkbox = index_qcheckbox_material_scenario[index];
	checkbox->setText(new_name);
	main_scenario.materials_scenarios_ids[index] = new_name.toStdString();
}

void MainWindow::removeMaterialScenario(int index){
	for(MaterialConfigUI* config: materials_ui)
		config->escenarioRemoved(index);
	QCheckBox* checkbox = index_qcheckbox_material_scenario[index];
	index_qcheckbox_material_scenario.erase(index);
	qcheckbox_material_scenario_index.erase(checkbox);
	ui->widget_materials->layout()->removeWidget(checkbox);
	delete checkbox;
	main_scenario.materials_scenarios_ids.erase(index);
}

void MainWindow::removeSeismicScenario(int index){
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	ui->widget_seismic->layout()->removeWidget(config);
	delete config;
	main_scenario.seismic_scenarios_ids.erase(index);
}

void MainWindow::changedSismicName(int index, QString new_name){
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	config->setNewName(new_name);
	main_scenario.seismic_scenarios_ids[index] = new_name.toStdString();
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
		for(MaterialConfigUI* material_ui: materials_ui){
			material_ui->toggleMaterial(index, toggled);
		}
	}
}
