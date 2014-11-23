#include "scenariomaterialsconfigui.h"
#include "ui_scenariomaterialsconfigui.h"
#include "materialconfigui.h"
#include "materialproperty.h"

ScenarioMaterialsConfigUI::ScenarioMaterialsConfigUI(QWidget *parent, int index, Material& material) :
	QWidget(parent),
	ui(new Ui::ScenarioMaterialsConfigUI),
	material(material)
{
	ui->setupUi(this);
	this->material_index = index;
	//this->read_only = read_only;
	for(MaterialProperty &property: material.properties){
		std::string material_value = property.getValue(MaterialProperty::ORIGINAL_VALUE);
		property.values[material_index] = material_value;
		QLineEdit* line_edit = new QLineEdit(QString::fromStdString(material_value));
		//line_edit->setReadOnly(read_only);
		line_edits_material[line_edit] = &property;
		QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->groupBox->layout());
		grid_layout->addRow(QString::fromStdString(property.name), line_edit);
		connect(line_edit, SIGNAL(textEdited(QString)), this, SLOT(updatePropertyValue(QString)));
	}
}

ScenarioMaterialsConfigUI::~ScenarioMaterialsConfigUI()
{
	delete ui;
}

void ScenarioMaterialsConfigUI::toggleProperty(int index, bool toggled){
	QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->groupBox->layout());
	grid_layout->itemAt(index, QFormLayout::FieldRole)->widget()->setVisible(toggled);
	grid_layout->itemAt(index, QFormLayout::LabelRole)->widget()->setVisible(toggled);
}

void ScenarioMaterialsConfigUI::updatePropertyValue(QString new_value){
	QLineEdit* line_edit = qobject_cast<QLineEdit *>(sender());
	MaterialProperty* property = line_edits_material[line_edit];
	property->values[material_index] = new_value.toStdString();
}
void ScenarioMaterialsConfigUI::setName(QString name){
	ui->groupBox->setTitle(name);
}
