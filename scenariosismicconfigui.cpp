#include "scenariosismicconfigui.h"
#include "ui_scenariosismicconfigui.h"

ScenarioSismicConfigUI::ScenarioSismicConfigUI(QWidget *parent, EscenarioSeismicCustom* scenario) :
	QWidget(parent),
	ui(new Ui::ScenarioSismicConfigUI)
{
	this->scenario = scenario;
	ui->setupUi(this);
	ui->lineEdit_seismic->setValue(scenario->seismic);
	ui->lineEdit_seismicv->setValue(scenario->seismicv);
	connect(ui->lineEdit_seismic, SIGNAL(valueChanged(double)), this, SLOT(changedSeismic()));
	connect(ui->lineEdit_seismicv, SIGNAL(valueChanged(double)), this, SLOT(changedSeismic()));
	connect(ui->checkBox, SIGNAL(toggled(bool)), this, SLOT(changedSeismic()));
}

ScenarioSismicConfigUI::~ScenarioSismicConfigUI()
{
	delete ui;
}

void ScenarioSismicConfigUI::changedSeismic(){
	scenario->seismic = ui->lineEdit_seismic->value();
	scenario->seismicv = ui->lineEdit_seismicv->value();
}
void ScenarioSismicConfigUI::setNewName(QString newName, QString abbr){
	scenario->name = newName.toStdWString();
	scenario->abbr = abbr.toStdString();
	ui->checkBox->setText("Generar Escenario " + newName + " (" + abbr + ")");
}

void ScenarioSismicConfigUI::toggled(bool toggled){
	scenario->enabled = toggled;
}
