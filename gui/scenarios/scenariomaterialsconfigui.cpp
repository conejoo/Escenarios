#include "scenariomaterialsconfigui.h"
#include "ui_scenariomaterialsconfigui.h"
#include "model/scenarios/materialproperty.h"
#include "gui/scenarios/materialconfigui.h"
#include "model/scenarios/strengthfunction.h"

#include <iostream>
#include <QLabel>
#include <QComboBox>
#include <QDoubleSpinBox>

ScenarioMaterialsConfigUI::ScenarioMaterialsConfigUI(QWidget *parent, int scenario_index, Material& material) :
	QWidget(parent),
	ui(new Ui::ScenarioMaterialsConfigUI),
	material(material)
{
	ui->setupUi(this);
	this->str_functions = 0;
	this->scenario_index = scenario_index;
	//this->read_only = read_only;
	QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->groupBox->layout());
	for(MaterialProperty &property: material.properties){
		if(!property.editable)
			continue;
		property.values[scenario_index] = property.getValue(MaterialProperty::ORIGINAL_VALUE);
		QDoubleSpinBox* line_edit = new QDoubleSpinBox();
		line_edit->setMinimum(0.0);
		if(property.short_name.compare("phi") == 0)
			line_edit->setMaximum(90.0);
		else
			line_edit->setMaximum(10000000.0);
		line_edit->setValue(property.values[scenario_index]);
		//QString::number(material_value)
		//line_edit->setReadOnly(read_only);
		line_edits_material[line_edit] = &property;
		property_line_edit[&property] = line_edit;
		if(scenario_index == MaterialProperty::ORIGINAL_VALUE)
			ui->groupBox->setStyleSheet("QGroupBox, QLabel { color : blue; }");
		grid_layout->addRow(QString::fromStdWString(property.name), line_edit);
		property_name_index[QString::fromStdString(property.short_name)] = grid_layout->rowCount() - 1;
		connect(line_edit, SIGNAL(valueChanged(double)), this, SLOT(updatePropertyValue(double)));
	}
	if (material.type == 6) {
		//str_functions = new QComboBox();
		grid_layout->addRow(
			QString("Strength Function"),
			new QLabel(QString::fromStdString(material.strength_fn))
		);

	}
}

ScenarioMaterialsConfigUI::~ScenarioMaterialsConfigUI()
{
	delete ui;
	for(auto& it: line_edits_material)
		delete it.first;
}

//void ScenarioMaterialsConfigUI::setupStrengthsFunctionsCombobox(QStringList &list) {
//	if (str_functions)
//		for (QString fn: list)
//			str_functions->addItem(fn);
//	int index = str_functions->findText(QString::fromStdString(material.strength_fn));
//	if( index != -1 )
//		str_functions->setCurrentIndex(index);
//}

void ScenarioMaterialsConfigUI::toggleProperty(QString name, bool toggled){
	if (property_name_index.find(name) != property_name_index.end()) {
		int index = property_name_index[name];
		QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->groupBox->layout());
		grid_layout->itemAt(index, QFormLayout::FieldRole)->widget()->setEnabled(toggled);
		grid_layout->itemAt(index, QFormLayout::LabelRole)->widget()->setEnabled(toggled);

	}
}

void ScenarioMaterialsConfigUI::updatePropertyValue(double new_value){
	QDoubleSpinBox* line_edit = qobject_cast<QDoubleSpinBox *>(sender());
	MaterialProperty* property = line_edits_material[line_edit];
	property->values[scenario_index] = new_value;
}

void ScenarioMaterialsConfigUI::setName(QString name){
	ui->groupBox->setTitle(name);
}

void ScenarioMaterialsConfigUI::applyPercentaje(double percentaje, QString property_short_name){
	int property_index = material.getPropertyIndex(property_short_name.toStdString());
	if (property_index != -1) {
		MaterialProperty& property = material.properties[property_index];
		double new_value = property.values[MaterialProperty::ORIGINAL_VALUE];
		new_value += new_value*(percentaje/100.0);
		QDoubleSpinBox* spin_box = property_line_edit[&property];
		if (spin_box) {
			spin_box->setValue(new_value);
			property.values[scenario_index] = spin_box->value();
		}
	}
}
