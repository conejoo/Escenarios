#include "scenariosismicconfigui.h"
#include "ui_scenariosismicconfigui.h"

ScenarioSismicConfigUI::ScenarioSismicConfigUI(QWidget *parent, EscenarioFile& scenario, int index) :
	QWidget(parent),
	ui(new Ui::ScenarioSismicConfigUI),
	scenario(scenario)
{
	ui->setupUi(this);
	this->index = index;
	ui->lineEdit_seismic->setText(QString::number(scenario.sesmic_escenarios[index]));
	ui->lineEdit_seismicv->setText(QString::number(scenario.sesmicv_escenarios[index]));
}

ScenarioSismicConfigUI::~ScenarioSismicConfigUI()
{
	delete ui;
}
