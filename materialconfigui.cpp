#include "materialconfigui.h"
#include "ui_materialconfigui.h"
#include <QLabel>

MaterialConfigUI::MaterialConfigUI(QWidget *parent, Material& material, int material_index, bool read_only) :
	QWidget(parent),
	ui(new Ui::MaterialConfigUI),
	material(material)
{
	ui->setupUi(this);
	this->material_index = material_index;
	this->read_only = read_only;
	ui->material_box->setTitle(QString::fromStdString(material.name));
	for(MaterialProperty &property: material.properties){
		std::string material_value = property.getValue(MaterialProperty::ORIGINAL_VALUE);
		property.values[material_index] = material_value;
		QLineEdit* line_edit = new QLineEdit(QString::fromStdString(material_value));
		line_edit->setReadOnly(read_only);
		QLabel* label = new QLabel(QString::fromStdString(property.name));
		line_edits_material[line_edit] = &property;
		ui->material_box->layout()->addWidget(label);
		ui->material_box->layout()->addWidget(line_edit);
		connect(line_edit, SIGNAL(textEdited(QString)), this, SLOT(updatePropertyValue(QString)));
	}
}

MaterialConfigUI::~MaterialConfigUI()
{
	delete ui;
}

void MaterialConfigUI::updatePropertyValue(QString new_value){
	QLineEdit* line_edit = qobject_cast<QLineEdit *>(sender());
	MaterialProperty* property = line_edits_material[line_edit];
	property->values[material_index] = new_value.toStdString();
}
