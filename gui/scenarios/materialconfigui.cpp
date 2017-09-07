#include "materialconfigui.h"
#include "ui_materialconfigui.h"
#include <QLabel>
#include <iostream>

MaterialConfigUI::MaterialConfigUI(QWidget *parent, Material& material) :
	QWidget(parent),
	ui(new Ui::MaterialConfigUI),
	material(material)
{
	ui->setupUi(this);
	ui->material_box->setTitle(QString::fromStdString(material.name));
}

MaterialConfigUI::~MaterialConfigUI()
{
	delete ui;
}

void MaterialConfigUI::escenarioRemoved(int index){
	ScenarioMaterialsConfigUI* config = material_scenarios[index];
	ui->material_box->layout()->removeWidget(config);
	material_scenarios.erase(index);
	delete config;
}

void MaterialConfigUI::escenarioAdded(int index, QString name){
	ScenarioMaterialsConfigUI* config = new ScenarioMaterialsConfigUI(ui->material_box, index, material);
	config->setName(name);
//	if (material.type == 6)
//		config->setupStrengthsFunctionsCombobox(str_functions_list);
	material_scenarios[index] = config;
	ui->material_box->layout()->addWidget(config);
}

void MaterialConfigUI::escenarioChangedName(int index, QString newname){
	ScenarioMaterialsConfigUI* config = material_scenarios[index];
	config->setName(newname);
}

void MaterialConfigUI::toggleProperty(QString name, bool toggled){
	for(auto it: material_scenarios){
		ScenarioMaterialsConfigUI* config = it.second;
		config->toggleProperty(name, toggled);
	}
}

void MaterialConfigUI::toggleScenario(int index, bool show){
	ScenarioMaterialsConfigUI* config = material_scenarios[index];
	if(config)
		config->setVisible(show);
}

void MaterialConfigUI::applyPercentaje(double percentaje, int scenario_index, int property_index){
	ScenarioMaterialsConfigUI* config = material_scenarios[scenario_index];
	if(config){
		config->applyPercentaje(percentaje, property_index);
	}
}

