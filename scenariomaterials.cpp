#include "scenariomaterials.h"
#include "ui_scenariomaterials.h"

ScenarioMaterials::ScenarioMaterials(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ScenarioMaterials)
{
	ui->setupUi(this);
}

ScenarioMaterials::~ScenarioMaterials()
{
	delete ui;
}
