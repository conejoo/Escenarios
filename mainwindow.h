#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "escenariofile.h"
#include <QMainWindow>
#include <unordered_map>
#include "scenariosconfigui.h"
class ScenarioMaterialsConfigUI;
class ScenarioSismicConfigUI;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		void openScenario(std::string);

	public slots:
		void openScenarioPromp();
		void exportScenariosPromp();
		void addMaterialScenario(int, QString);
		void addSismicScenario(int, QString);
		void changedMaterialName(int index, QString new_name);
		void changedSismicName(int, QString);
		void removeMaterialScenario(int index);
		void removeSeismicScenario(int);

	private:
		int findTabIndex(QWidget* widget, QTabWidget *tab_widget);
		void removeAllTabs(QTabWidget* tabs);
		Ui::MainWindow *ui;
		ScenarioMaterialsConfigUI* main_scenario_ui;
		std::unordered_map<int, ScenarioMaterialsConfigUI*> custom_material_schenarios_ui;
		std::unordered_map<int, ScenarioSismicConfigUI*> custom_seismic_schenarios_ui;
		EscenarioFile main_scenario;
		ScenariosConfigUI scenarios_config;
};

#endif // MAINWINDOW_H
