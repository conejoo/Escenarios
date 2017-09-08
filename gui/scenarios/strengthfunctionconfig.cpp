#include "strengthfunctionconfig.h"
#include "ui_strengthfunctionconfig.h"
#include "scenariostrengthfunctionconfig.h"
#include "gui/scenarios/scenariostrengthfunctionconfig.h"
#include "model/scenarios/strengthfunction.h"

StrengthFunctionConfig::StrengthFunctionConfig(QWidget *parent, StrengthFunction *strength_function) :
	QWidget(parent),
	ui(new Ui::StrengthFunctionConfig)
{
	this->ui->setupUi(this);
	this->ui->groupBox->setTitle(QString::fromStdString(strength_function->name));
	this->strength_function = strength_function;
}

StrengthFunctionConfig::~StrengthFunctionConfig()
{
	delete ui;
}


void StrengthFunctionConfig::escenarioRemoved(int index){
	ScenarioStrengthFunctionConfig *config = strength_function_scenarios[index];
	ui->groupBox->layout()->removeWidget(config);
	strength_function_scenarios.erase(index);
	delete config;
}

void StrengthFunctionConfig::escenarioAdded(int index, QString name){
	ScenarioStrengthFunctionConfig* config = new ScenarioStrengthFunctionConfig(ui->groupBox, strength_function);
	config->setName(name);
	strength_function_scenarios[index] = config;
	ui->groupBox->layout()->addWidget(config);
}

void StrengthFunctionConfig::toggleScenario(int index, bool show){
	ScenarioStrengthFunctionConfig* config = strength_function_scenarios[index];
	if(config)
		config->setVisible(show);
}

void StrengthFunctionConfig::escenarioChangedName(int index, QString newname){
	ScenarioStrengthFunctionConfig* config = strength_function_scenarios[index];
	config->setName(newname);
}

void StrengthFunctionConfig::applyPercentaje(double percentaje, int scenario_index, QString property_short_name){
	if (strength_function_scenarios.find(scenario_index) != strength_function_scenarios.end()) {
		ScenarioStrengthFunctionConfig* config = strength_function_scenarios[scenario_index];
		if (config)
			config->applyPercentaje(percentaje, property_short_name);
	}
}

void StrengthFunctionConfig::toggleProperty(QString name, bool toggled){
	for(auto it: strength_function_scenarios){
		ScenarioStrengthFunctionConfig* config = it.second;
		config->toggleProperty(name, toggled);
	}
}
