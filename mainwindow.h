#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <unordered_map>
#include <fstream>
#include "model/scenarios/escenariofile.h"
#include "gui/scenarios/scenariosconfigui.h"
#include "gui/scenarios/materialgeneralconfig.h"
#include "gui/results/resultsprocessui.h"
#include "gui/aboutrtb.h"

class MaterialConfigUI;
class StrengthFunctionConfig;
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
		void addMaterialScenario(int, QString, QString);
		void addProperties();
		void addMaterials();
		void addStrengthFunctions();
		void addSismicScenario(int, QString, QString);
		void changedMaterialScenarioName(int index, QString new_name);
		void changedMaterialScenarioAbbr(int index, QString abbr);
		void changedSismicName(int, QString);
		void changedSismicAbbr(int, QString abbr);
		void removeMaterialScenario(int index);
		void removeSeismicScenario(int);
		void toggleProperty(bool);
		void toggleMaterialScenario(bool);
		void applyPercentaje(double percent, int scenario_index, QString property_short_name);

	private:
		Ui::MainWindow *ui;
		std::vector<StrengthFunctionConfig*> strength_functions_ui;
		std::vector<MaterialConfigUI*> materials_ui;
		std::unordered_map<int, ScenarioSismicConfigUI*> custom_seismic_schenarios_ui;
		std::unordered_map<QCheckBox*, MaterialProperty*> qcheckbox_property_index;
		std::unordered_map<int, QCheckBox*> index_qcheckbox_material_scenario;
		std::unordered_map<QCheckBox*, int> qcheckbox_material_scenario_index;
		EscenarioFile main_scenario;
		AboutRTB about;
		ScenariosConfigUI scenarios_config;
		ResultsProcessUI result_process_ui;
		MaterialGeneralConfig general_material_config;
		void printFileParametersLine(std::wofstream& file, EscenarioSeismicCustom* seismic_es, std::string property_short_name, int scenario_index, QString complete_filename, std::vector<std::string>& type_0_property_short_names);
};

#endif // MAINWINDOW_H
