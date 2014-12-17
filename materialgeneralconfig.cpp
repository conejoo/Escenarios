#include "materialgeneralconfig.h"
#include "ui_materialgeneralconfig.h"
#include "materialescenariosgeneralconfig.h"
#include "escenariomaterialcustom.h"

MaterialGeneralConfig::MaterialGeneralConfig(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MaterialGeneralConfig)
{
	ui->setupUi(this);
}

MaterialGeneralConfig::~MaterialGeneralConfig()
{
	delete ui;
}

void MaterialGeneralConfig::clearScenarios(){
	QLayoutItem* item;
	while((item = ui->groupBox->layout()->takeAt(0))){
		delete item;
	}
}

void MaterialGeneralConfig::addScenario(EscenarioMaterialCustom* scenario, Material& m){
	MaterialEscenariosGeneralConfig* scenario_config =
			new MaterialEscenariosGeneralConfig(this, m, scenario->index);
	scenario_config->setName(QString::fromStdWString(scenario->name));
	configs[scenario->index] = scenario_config;
	ui->groupBox->layout()->addWidget(scenario_config);
	connect(scenario_config, SIGNAL(percentajeApplied(double,int,int)),
			this, SIGNAL(percentajeApplied(double,int,int)));
}


void MaterialGeneralConfig::escenarioRemoved(int index){
	MaterialEscenariosGeneralConfig* config = configs[index];
	ui->groupBox->layout()->removeWidget(config);
	configs.erase(index);
	delete config;
}

void MaterialGeneralConfig::escenarioChangedName(int index, QString newname){
	MaterialEscenariosGeneralConfig* config = configs[index];
	config->setName(newname);
}

void MaterialGeneralConfig::toggleProperty(int index, bool toggled){
	for(auto it: configs){
		MaterialEscenariosGeneralConfig* config = it.second;
		config->toggleProperty(index, toggled);
	}
}
void MaterialGeneralConfig::toggleMaterial(int index, bool show){
	MaterialEscenariosGeneralConfig* config = configs[index];
	if(config)
		config->setVisible(show);
}
