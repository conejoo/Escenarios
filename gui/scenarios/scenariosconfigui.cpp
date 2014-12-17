#include "scenariosconfigui.h"
#include "ui_scenariosconfigui.h"
#include <QFormLayout>
#include <sstream>

#include "model/scenarios/escenariofile.h"
#include "model/scenarios/materialproperty.h"

ScenariosConfigUI::ScenariosConfigUI(QWidget *parent) :
	QWidget(parent, Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint),
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
	int index_base = MaterialProperty::ORIGINAL_VALUE;
	addMaterialScenario(index_base, QString("Escenario Medio"), QString("med"));
	addMaterialScenario(1, QString("Escenario Mínimo"), QString("min"));
	addMaterialScenario(3, QString("Escenario Máximo"), QString("max"));
	addSismicScenario(index_base, QString("Estático"), QString("base"));
	addSismicScenario(7, QString("Sismo Operacional"), QString("op"));
	addSismicScenario(8, QString("Abandono"), QString("ab"));
	ui->pushButton_new_material->setEnabled(true);
	ui->pushButton_new_sismic->setEnabled(true);
}

void ScenariosConfigUI::addNewMaterialScenario(){
	scenarios_index++;
	std::ostringstream stringStream;
	stringStream << "ms_" << scenarios_index;
	addMaterialScenario(scenarios_index, "Escenario Material", QString::fromStdString(stringStream.str()));
}

void ScenariosConfigUI::addNewSismicScenario(){
	scenarios_index++;
	std::ostringstream stringStream;
	stringStream << "ss_" << scenarios_index;
	addSismicScenario(scenarios_index, "Escenario Sísmico", QString::fromStdString(stringStream.str()));
}
#include <iostream>
void ScenariosConfigUI::addMaterialScenario(int index, QString name, QString abbr){
	bool original = index == MaterialProperty::ORIGINAL_VALUE;
	QLineEdit* line_edit = new QLineEdit(name);
	QLineEdit* line_edit_abbr = new QLineEdit(abbr);
	if(original){
		line_edit_abbr->setStyleSheet("QLineEdit { color : blue; }");
		line_edit->setStyleSheet("QLineEdit { color : blue; }");
	}
	QPushButton* button = new QPushButton(QString("Remover"));
	button->setEnabled(!original);
	connect(button, SIGNAL(clicked()), this, SLOT(removeMaterialScenario()));
	connect(line_edit, SIGNAL(textEdited(QString)), this, SLOT(changedNameMaterial(QString)));
	connect(line_edit_abbr, SIGNAL(textEdited(QString)), this, SLOT(changedAbbrMaterial(QString)));
	line_edits_material[index] = line_edit;
	line_edits_abbr_material[index] = line_edit_abbr;
	remove_buttons_material[button] = index;
	line_edits_material_inv[line_edit] = index;
	line_edits_material_inv[line_edit_abbr] = index;
	QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->widget_material_configs->layout());
	int cols = grid_layout->rowCount();
	grid_layout->addWidget(line_edit, cols, 0);
	grid_layout->addWidget(line_edit_abbr, cols, 1);
	grid_layout->addWidget(button, cols, 2);
	emit addedMaterialScenario(index, name, abbr);
}

void ScenariosConfigUI::addSismicScenario(int index, QString name, QString abbr){
	bool original = index == MaterialProperty::ORIGINAL_VALUE;
	QLineEdit* line_edit = new QLineEdit(name);
	QLineEdit* line_edit_abbr = new QLineEdit(abbr);
	QPushButton* button = new QPushButton(QString("Remover"));
	button->setEnabled(!original);
	connect(button, SIGNAL(clicked()), this, SLOT(removeSismicScenario()));
	connect(line_edit, SIGNAL(textEdited(QString)), this, SLOT(changedNameSismic(QString)));
	connect(line_edit_abbr, SIGNAL(textEdited(QString)), this, SLOT(changedAbbrSismic(QString)));
	line_edits_sismic[index] = line_edit;
	line_edits_abbr_sismic[index] = line_edit_abbr;
	remove_buttons_sismic[button] = index;
	line_edits_sismic_inv[line_edit] = index;
	line_edits_sismic_inv[line_edit_abbr] = index;
	QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->widget_sismic_configs->layout());
	int cols = grid_layout->rowCount();
	grid_layout->addWidget(line_edit, cols, 0);
	grid_layout->addWidget(line_edit_abbr, cols, 1);
	grid_layout->addWidget(button, cols, 2);
	emit addedSismicScenario(index, name, abbr);
}

void ScenariosConfigUI::removeSismicScenario(){
	QPushButton* button = qobject_cast<QPushButton *>(sender());
	int key = remove_buttons_sismic[button];
	QLineEdit* label = line_edits_sismic[key];
	QLineEdit* label_abbr = line_edits_abbr_sismic[key];
	QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->widget_sismic_configs->layout());
	grid_layout->removeWidget(label);
	grid_layout->removeWidget(button);
	grid_layout->removeWidget(label_abbr);
	delete button;
	delete label;
	delete label_abbr;
	emit removedSismicScenario(key);
}


void ScenariosConfigUI::removeMaterialScenario(){
	QPushButton* button = qobject_cast<QPushButton *>(sender());
	int key = remove_buttons_material[button];
	QLineEdit* label = line_edits_material[key];
	QLineEdit* label_abbr = line_edits_abbr_material[key];
	QGridLayout * grid_layout = qobject_cast<QGridLayout *>(ui->widget_material_configs->layout());
	grid_layout->removeWidget(label);
	grid_layout->removeWidget(button);
	grid_layout->removeWidget(label_abbr);
	delete button;
	delete label;
	delete label_abbr;
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

void ScenariosConfigUI::changedAbbrSismic(QString newText){
	QLineEdit* line_edit = qobject_cast<QLineEdit *>(sender());
	int key = line_edits_sismic_inv[line_edit];
	emit changedAbbrSismicScenario(key, newText);
}

void ScenariosConfigUI::changedAbbrMaterial(QString newText){
	QLineEdit* line_edit = qobject_cast<QLineEdit *>(sender());
	int key = line_edits_material_inv[line_edit];
	emit changedAbbrMaterialScenario(key, newText);
}
