#include "materialescenariosgeneralconfig.h"
#include "ui_materialescenariosgeneralconfig.h"
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "model/scenarios/materialproperty.h"

MaterialEscenariosGeneralConfig::MaterialEscenariosGeneralConfig(QWidget *parent, Material& material, int index):
	QWidget(parent),
	ui(new Ui::MaterialEscenariosGeneralConfig)
{
	ui->setupUi(this);
	this->scenario_index = index;
	//this->read_only = read_only;
	QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->groupBox->layout());
	row = 0;
	for(int property_index = 0; property_index < (int)material.properties.size(); property_index++){
		MaterialProperty &property = material.properties[property_index];
		if(!property.editable)
			continue;
		double material_value = property.getValue(MaterialProperty::ORIGINAL_VALUE);
		property.values[scenario_index] = material_value;
		QDoubleSpinBox* line_edit = new QDoubleSpinBox();
		line_edit->setSuffix(QString(" %"));
		line_edit->setMinimum(-10000000.0);
		line_edit->setMaximum(10000000.0);
		line_edit->setValue(0.0);
		//QString::number(material_value)
		//line_edit->setReadOnly(read_only);
		//line_edits_material[line_edit] = &property;
		if(scenario_index == MaterialProperty::ORIGINAL_VALUE)
			ui->groupBox->setStyleSheet("QGroupBox, QLabel { color : blue; }");
		QPushButton *pbutton = new QPushButton("Aplicar");
		grid_layout->addWidget(new QLabel(QString::fromStdWString(property.name)), row, 0);
		grid_layout->addWidget(line_edit,row, 1);
		grid_layout->addWidget(pbutton,row, 2);
		buttons_map[pbutton] = std::pair<int, QDoubleSpinBox*>(property_index, line_edit);
		connect(pbutton, SIGNAL(clicked()), this, SLOT(applyPercentaje()));
		row++;
	}
}

MaterialEscenariosGeneralConfig::~MaterialEscenariosGeneralConfig()
{
	delete ui;
}

void MaterialEscenariosGeneralConfig::setName(QString new_name){
	ui->groupBox->setTitle(new_name);
}

void MaterialEscenariosGeneralConfig::toggleProperty(int index, bool toggled){
	QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->groupBox->layout());
	grid_layout->itemAtPosition(index, 0)->widget()->setEnabled(toggled);
	grid_layout->itemAtPosition(index, 1)->widget()->setEnabled(toggled);
	grid_layout->itemAtPosition(index, 2)->widget()->setEnabled(toggled);
}

void MaterialEscenariosGeneralConfig::applyPercentaje(){
	QPushButton * button = qobject_cast<QPushButton *>(sender());
	std::pair<int, QDoubleSpinBox*>& res = buttons_map[button];
	emit percentajeApplied(res.second->value(), scenario_index, res.first);
}
