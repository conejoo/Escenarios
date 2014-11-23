#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "escenariofile.h"
#include <QMainWindow>
#include <unordered_map>
#include "scenariosconfigui.h"
#include "resultsprocessui.h"
class MaterialConfigUI;
class ScenarioSismicConfigUI;
class QCheckBox;
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
		void clearLayout(QLayout *layout);
	public slots:
		void openScenarioPromp();
		void exportScenariosPromp();
		void addMaterialScenario(int, QString);
		void addProperties();
		void addMaterials();
		void addSismicScenario(int, QString);
		void changedMaterialScenarioName(int index, QString new_name);
		void changedSismicName(int, QString);
		void removeMaterialScenario(int index);
		void removeSeismicScenario(int);
		void toggleProperty(bool);
		void toggleMaterialScenario(bool);

	private:
		Ui::MainWindow *ui;
		std::vector<MaterialConfigUI*> materials_ui;
		std::unordered_map<int, ScenarioSismicConfigUI*> custom_seismic_schenarios_ui;
		std::unordered_map<QCheckBox*, MaterialProperty*> qcheckbox_property_index;
		std::unordered_map<int, QCheckBox*> index_qcheckbox_material_scenario;
		std::unordered_map<QCheckBox*, int> qcheckbox_material_scenario_index;
		EscenarioFile main_scenario;
		ScenariosConfigUI scenarios_config;
		ResultsProcessUI result_process_ui;
};

#endif // MAINWINDOW_H
