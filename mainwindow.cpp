#include "mainwindow.h"
#include <QFileDialog>
#include "ui_mainwindow.h"
#include "escenariofile.h"
#include "scenariomaterialsconfigui.h"
#include "scenariosismicconfigui.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	scenarios_config()
{
	ui->setupUi(this);
	connect(this->ui->actionOpen_Scenario, SIGNAL(triggered()), this, SLOT(openScenarioPromp()));
	connect(this->ui->actionExport, SIGNAL(triggered()), this, SLOT(exportScenariosPromp()));
	connect(this->ui->actionConfigurate_Scenarios, SIGNAL(triggered()), &this->scenarios_config, SLOT(show()));
	connect(&this->scenarios_config, SIGNAL(addedMaterialScenario(int,QString)), this, SLOT(addMaterialScenario(int,QString)));
	connect(&this->scenarios_config, SIGNAL(changedNameMaterialScenario(int,QString)), this, SLOT(changedMaterialName(int,QString)));
	connect(&this->scenarios_config, SIGNAL(removedMaterialScenario(int)), this, SLOT(removeMaterialScenario(int)));
	connect(&this->scenarios_config, SIGNAL(addedSismicScenario(int,QString)), this, SLOT(addSismicScenario(int,QString)));
	connect(&this->scenarios_config, SIGNAL(changedNameSismicScenario(int,QString)), this, SLOT(changedSismicName(int,QString)));
	connect(&this->scenarios_config, SIGNAL(removedSismicScenario(int)), this, SLOT(removeSeismicScenario(int)));
	std::string fixed_se = std::string("D:/Desarrollo/build-Escenarios-Desktop_Qt_5_3_MinGW_32bit-Debug/data/pilas_lix.sli");
	openScenario(fixed_se);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openScenarioPromp(){

	QString filename = QFileDialog::getOpenFileName(this,
													("Open File"), "",
													"Scenarios (*.sli)");
	openScenario(filename.toStdString());
}

void MainWindow::exportScenariosPromp(){
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
												"/home",
												QFileDialog::ShowDirsOnly
												| QFileDialog::DontResolveSymlinks);
	//for()
}

void MainWindow::openScenario(std::string filename){
	main_scenario = EscenarioFile(filename);
	removeAllTabs(ui->tabWidget);
	removeAllTabs(ui->tabWidget_sesmic);
	scenarios_config.setScenarioFile(&main_scenario);
}

void MainWindow::removeAllTabs(QTabWidget* tabs){
	while(true){
		QWidget* widget = tabs->widget(0);
		if(widget){
			tabs->removeTab(0);
			delete widget;
		}else
			return;
	}
}

void MainWindow::addMaterialScenario(int index, QString name){
	ScenarioMaterialsConfigUI* config = new ScenarioMaterialsConfigUI(this, main_scenario.materials, index);
	custom_material_schenarios_ui[index] = config;
	ui->tabWidget->addTab(config, name);
	main_scenario.materials_scenarios_ids.insert(index);
}

void MainWindow::addSismicScenario(int index, QString name){
	ScenarioSismicConfigUI* config = new ScenarioSismicConfigUI(this, main_scenario, index);
	custom_seismic_schenarios_ui[index] = config;
	ui->tabWidget_sesmic->addTab(config, name);
	main_scenario.seismic_scenarios_ids.insert(index);
}

int MainWindow::findTabIndex(QWidget* widget, QTabWidget* tab_widget){
	for(int i = 0; i < 100; i++){
		QWidget* tab_ = tab_widget->widget(i);
		if(!tab_)
			return -1;
		if(tab_ == widget)
			return i;
	}
	return -1;
}

void MainWindow::changedMaterialName(int index, QString new_name){
	ScenarioMaterialsConfigUI* config = custom_material_schenarios_ui[index];
	int tab_position = findTabIndex(config, ui->tabWidget);
	ui->tabWidget->setTabText(tab_position, new_name);
}

void MainWindow::removeMaterialScenario(int index){
	ScenarioMaterialsConfigUI* config = custom_material_schenarios_ui[index];
	int tab_position = findTabIndex(config, ui->tabWidget);
	ui->tabWidget->removeTab(tab_position);
	main_scenario.materials_scenarios_ids.remove(index);
}

void MainWindow::removeSeismicScenario(int index){
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	int tab_position = findTabIndex(config, ui->tabWidget_sesmic);
	ui->tabWidget_sesmic->removeTab(tab_position);
	main_scenario.seismic_scenarios_ids.remove(index);
}

void MainWindow::changedSismicName(int index, QString new_name){
	ScenarioSismicConfigUI* config = custom_seismic_schenarios_ui[index];
	int tab_position = findTabIndex(config, ui->tabWidget_sesmic);
	ui->tabWidget_sesmic->setTabText(tab_position, new_name);
}

