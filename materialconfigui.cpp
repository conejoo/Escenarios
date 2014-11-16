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
	for(MaterialProperty property: material.properties){
		QLineEdit* line_edit = new QLineEdit(QString::number(property.getValue(MaterialProperty::ORIGINAL_VALUE)));
		line_edit->setReadOnly(read_only);
		QLabel* label = new QLabel(QString::fromStdString(property.name));
		line_edits.push_back(line_edit);
		ui->material_box->layout()->addWidget(label);
		ui->material_box->layout()->addWidget(line_edit);
	}
}

MaterialConfigUI::~MaterialConfigUI()
{
	delete ui;
}
