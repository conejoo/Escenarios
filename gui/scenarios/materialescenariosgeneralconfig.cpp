#include "materialescenariosgeneralconfig.h"
#include "ui_materialescenariosgeneralconfig.h"
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "model/scenarios/materialproperty.h"

MaterialEscenariosGeneralConfig::MaterialEscenariosGeneralConfig(QWidget *parent, Material& material, int scenario_index):
	QWidget(parent),
	ui(new Ui::MaterialEscenariosGeneralConfig)
{
	ui->setupUi(this);
	this->scenario_index = scenario_index;
	//this->read_only = read_only;
	QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->groupBox->layout());
	row = 0;
	for(int property_index = 0; property_index < (int)material.properties.size(); property_index++){
		MaterialProperty &property = material.properties[property_index];
		if(!property.editable)
			continue;
		property.values[scenario_index] = property.getValue(MaterialProperty::ORIGINAL_VALUE);
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
		property_name_index[QString::fromStdString(property.short_name)] = row;
		buttons_map[pbutton] = QPair<QString, QDoubleSpinBox*>(QString::fromStdString(property.short_name), line_edit);
		connect(pbutton, SIGNAL(clicked()), this, SLOT(applyPercentaje()));
		row++;
	}
}

MaterialEscenariosGeneralConfig::~MaterialEscenariosGeneralConfig()
{
	delete ui;
}

void MaterialEscenariosGeneralConfig::addStrengthFunctionProperties(){
	QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->groupBox->layout());
	int row = grid_layout->rowCount();
	QDoubleSpinBox* line_edit = new QDoubleSpinBox();
	line_edit->setSuffix(QString(" °"));
	line_edit->setMinimum(-10000000.0);
	line_edit->setMaximum(10000000.0);
	line_edit->setValue(0.0);
	QPushButton *pbutton = new QPushButton("Aplicar");
	grid_layout->addWidget(new QLabel("Angulo (ang)"), row, 0);
	grid_layout->addWidget(line_edit,row, 1);
	grid_layout->addWidget(pbutton,row, 2);
	property_name_index[QString("ang")] = row;
	buttons_map[pbutton] = QPair<QString, QDoubleSpinBox*>(QString("ang"), line_edit);
	connect(pbutton, SIGNAL(clicked()), this, SLOT(applyPercentaje()));
}

void MaterialEscenariosGeneralConfig::setName(QString new_name){
	ui->groupBox->setTitle(new_name);
}

void MaterialEscenariosGeneralConfig::toggleProperty(QString name, bool toggled){
	if (property_name_index.find(name) != property_name_index.end()) {
		int index = property_name_index[name];
		QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->groupBox->layout());
		grid_layout->itemAtPosition(index, 0)->widget()->setEnabled(toggled);
		grid_layout->itemAtPosition(index, 1)->widget()->setEnabled(toggled);
		grid_layout->itemAtPosition(index, 2)->widget()->setEnabled(toggled);
	}
}

void MaterialEscenariosGeneralConfig::applyPercentaje(){
	QPushButton * button = qobject_cast<QPushButton *>(sender());
	if (buttons_map.find(button) != buttons_map.end()) {
		QPair<QString, QDoubleSpinBox*>& res = buttons_map[button];
		emit percentajeApplied(res.second->value(), scenario_index, res.first);
	}
}
