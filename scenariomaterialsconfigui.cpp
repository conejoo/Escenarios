#include "scenariomaterialsconfigui.h"
#include "ui_scenariomaterialsconfigui.h"
#include "materialconfigui.h"
#include "materialproperty.h"

ScenarioMaterialsConfigUI::ScenarioMaterialsConfigUI(QWidget *parent, std::vector<Material>& materials, int index) :
	QWidget(parent),
	ui(new Ui::ScenarioMaterialsConfigUI),
	materials(materials)
{
	ui->setupUi(this);
	this->read_only = index == MaterialProperty::ORIGINAL_VALUE;
	this->index = index;
	for(Material& m: materials){
		MaterialConfigUI *material_ui = new MaterialConfigUI(this, m, index, read_only);
		ui->scrollAreaWidgetContents->layout()->addWidget(material_ui);
	}
}

ScenarioMaterialsConfigUI::~ScenarioMaterialsConfigUI()
{
	delete ui;
}
