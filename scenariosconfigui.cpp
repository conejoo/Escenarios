#include "scenariosconfigui.h"
#include "ui_scenariosconfigui.h"
#include "escenariofile.h"
#include "materialproperty.h"
#include <QFormLayout>

ScenariosConfigUI::ScenariosConfigUI(QWidget *parent) :
	QWidget(parent, Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint),
	ui(new Ui::ScenariosConfigUI)
{
	ui->setupUi(this);
	connect(ui->pushButton_new_material, SIGNAL(clicked()), this, SLOT(addNewMaterialScenario()));
	connect(ui->pushButton_new_sismic, SIGNAL(clicked()), this, SLOT(addNewSismicScenario()));
	scenarios_index = 1;
}

ScenariosConfigUI::~ScenariosConfigUI()
{
	delete ui;
}

void ScenariosConfigUI::setScenarioFile(EscenarioFile* file)
{
	scenarios_index = 10;
	scenarios = file;
	QLayoutItem *child;
	while ((child = ui->widget_material_configs->layout()->takeAt(0)) != 0){
		if(child->widget() != 0)
			delete child->widget();
		delete child;
	}
	while ((child = ui->widget_sismic_configs->layout()->takeAt(0)) != 0){
		if(child->widget() != 0)
			delete child->widget();
		delete child;
	}
	line_edits_material.clear();
	remove_buttons_material.clear();
	line_edits_material_inv.clear();
	line_edits_sismic.clear();
	remove_buttons_sismic.clear();
	line_edits_sismic_inv.clear();
	addMaterialScenario(1, QString("Escenario Bajo"));
	addMaterialScenario(2, QString("Escenario Medio"));
	addMaterialScenario(3, QString("Escenario Alto"));
	addSismicScenario(4, QString("Sismo Operacional"));
	addSismicScenario(5, QString("Abandono"));
}

void ScenariosConfigUI::addNewMaterialScenario(){
	scenarios_index++;
	addMaterialScenario(scenarios_index, "New name");
}

void ScenariosConfigUI::addNewSismicScenario(){
	scenarios_index++;
	addSismicScenario(scenarios_index, "New name");
}
#include <iostream>
void ScenariosConfigUI::addMaterialScenario(int index, QString name){
	bool original = index == MaterialProperty::ORIGINAL_VALUE;
	QLineEdit* line_edit = new QLineEdit(name);
	QPushButton* button = new QPushButton(QString("Remove"));
	button->setEnabled(!original);
	connect(button, SIGNAL(clicked()), this, SLOT(removeMaterialScenario()));
	connect(line_edit, SIGNAL(textEdited(QString)), this, SLOT(changedNameMaterial(QString)));
	line_edits_material[index] = line_edit;
	remove_buttons_material[button] = index;
	line_edits_material_inv[line_edit] = index;
	QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->widget_material_configs->layout());
	grid_layout->addRow(line_edit, button);
	emit addedMaterialScenario(index, name);
}

void ScenariosConfigUI::addSismicScenario(int index, QString name){
	bool original = index == MaterialProperty::ORIGINAL_VALUE;
	QLineEdit* line_edit = new QLineEdit(name);
	QPushButton* button = new QPushButton(QString("Remove"));
	button->setEnabled(!original);
	connect(button, SIGNAL(clicked()), this, SLOT(removeSismicScenario()));
	connect(line_edit, SIGNAL(textEdited(QString)), this, SLOT(changedNameSismic(QString)));
	line_edits_sismic[index] = line_edit;
	remove_buttons_sismic[button] = index;
	line_edits_sismic_inv[line_edit] = index;
	QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->widget_sismic_configs->layout());
	grid_layout->addRow(line_edit, button);
	emit addedSismicScenario(index, name);
}

void ScenariosConfigUI::removeSismicScenario(){
	QPushButton* button = qobject_cast<QPushButton *>(sender());
	int key = remove_buttons_sismic[button];
	QLineEdit* label = line_edits_sismic[key];
	QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->widget_sismic_configs->layout());
	grid_layout->removeWidget(label);
	grid_layout->removeWidget(button);
	delete button;
	delete label;
	emit removedSismicScenario(key);
}


void ScenariosConfigUI::removeMaterialScenario(){
	QPushButton* button = qobject_cast<QPushButton *>(sender());
	int key = remove_buttons_material[button];
	QLineEdit* label = line_edits_material[key];
	QFormLayout * grid_layout = qobject_cast<QFormLayout *>(ui->widget_material_configs->layout());
	grid_layout->removeWidget(label);
	grid_layout->removeWidget(button);
	delete button;
	delete label;
	emit removedMaterialScenario(key);
}

void ScenariosConfigUI::changedNameMaterial(QString newText){
	QLineEdit* line_edit = qobject_cast<QLineEdit *>(sender());
	int key = line_edits_material_inv[line_edit];
	emit changedNameMaterialScenario(key, newText);
}

void ScenariosConfigUI::changedNameSismic(QString newText){
	QLineEdit* line_edit = qobject_cast<QLineEdit *>(sender());
	int key = line_edits_sismic_inv[line_edit];
	emit changedNameSismicScenario(key, newText);
}
