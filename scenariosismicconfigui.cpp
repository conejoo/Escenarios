#include "scenariosismicconfigui.h"
#include "ui_scenariosismicconfigui.h"

ScenarioSismicConfigUI::ScenarioSismicConfigUI(QWidget *parent, EscenarioFile& scenario, int index) :
	QWidget(parent),
	ui(new Ui::ScenarioSismicConfigUI),
	scenario(scenario)
{
	ui->setupUi(this);
	this->index = index;
	if(index != MaterialProperty::ORIGINAL_VALUE){
		int base_index = MaterialProperty::ORIGINAL_VALUE;
		scenario.sesmic_escenarios[index] = scenario.sesmic_escenarios[base_index];
		scenario.sesmicv_escenarios[index] = scenario.sesmicv_escenarios[base_index];
	}
	ui->lineEdit_seismic->setText(QString::fromStdString(scenario.sesmic_escenarios[index]));
	ui->lineEdit_seismicv->setText(QString::fromStdString(scenario.sesmicv_escenarios[index]));
	connect(ui->lineEdit_seismic, SIGNAL(textEdited(QString)), this, SLOT(changedSeismic()));
	connect(ui->lineEdit_seismicv, SIGNAL(textEdited(QString)), this, SLOT(changedSeismic()));
}

ScenarioSismicConfigUI::~ScenarioSismicConfigUI()
{
	delete ui;
}

void ScenarioSismicConfigUI::changedSeismic(){
	scenario.sesmic_escenarios[index] = ui->lineEdit_seismic->text().toStdString();
	scenario.sesmicv_escenarios[index] = ui->lineEdit_seismicv->text().toStdString();
}
