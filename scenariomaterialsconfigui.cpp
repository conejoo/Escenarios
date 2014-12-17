#include "scenariomaterialsconfigui.h"
#include "ui_scenariomaterialsconfigui.h"
#include "materialconfigui.h"
#include "materialproperty.h"
#include <iostream>
#include <QLabel>

ScenarioMaterialsConfigUI::ScenarioMaterialsConfigUI(QWidget *parent, int index, Material& material) :
	QWidget(parent),
	ui(new Ui::ScenarioMaterialsConfigUI),
	material(material)
{
	ui->setupUi(this);
	this->material_index = index;
	//this->read_only = read_only;
	QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->groupBox->layout());
	for(MaterialProperty &property: material.properties){
		if(!property.editable)
			continue;
		double material_value = property.getValue(MaterialProperty::ORIGINAL_VALUE);
		property.values[material_index] = material_value;
		QDoubleSpinBox* line_edit = new QDoubleSpinBox();
		line_edit->setMinimum(0.0);
		if(property.short_name.compare("phi") == 0)
			line_edit->setMaximum(90.0);
		else
			line_edit->setMaximum(10000000.0);
		line_edit->setValue(material_value);
		//QString::number(material_value)
		//line_edit->setReadOnly(read_only);
		line_edits_material[line_edit] = &property;
		property_line_edit[&property] = line_edit;
		if(material_index == MaterialProperty::ORIGINAL_VALUE)
			ui->groupBox->setStyleSheet("QGroupBox, QLabel { color : blue; }");
		grid_layout->addRow(QString::fromStdString(property.name), line_edit);
		connect(line_edit, SIGNAL(valueChanged(double)), this, SLOT(updatePropertyValue(double)));
	}
}

ScenarioMaterialsConfigUI::~ScenarioMaterialsConfigUI()
{
	delete ui;
	for(auto& it: line_edits_material)
		delete it.first;
}

void ScenarioMaterialsConfigUI::toggleProperty(int index, bool toggled){
	QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->groupBox->layout());
	grid_layout->itemAt(index, QFormLayout::FieldRole)->widget()->setEnabled(toggled);
	grid_layout->itemAt(index, QFormLayout::LabelRole)->widget()->setEnabled(toggled);
}

void ScenarioMaterialsConfigUI::updatePropertyValue(double new_value){
	QDoubleSpinBox* line_edit = qobject_cast<QDoubleSpinBox *>(sender());
	MaterialProperty* property = line_edits_material[line_edit];
	property->values[material_index] = new_value;
}
void ScenarioMaterialsConfigUI::setName(QString name){
	ui->groupBox->setTitle(name);
}
void ScenarioMaterialsConfigUI::applyPercentaje(double percentaje, int property_index){
	MaterialProperty& property = material.properties[property_index];
	int orig_index = MaterialProperty::ORIGINAL_VALUE;
	double new_value = property.values[orig_index];
	new_value += new_value*(percentaje/100.0);
	QDoubleSpinBox* spin_box = property_line_edit[&property];
	if(spin_box){
		spin_box->setValue(new_value);
		property.values[material_index] = spin_box->value();
	}
}
