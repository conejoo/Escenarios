#ifndef SCENARIOSCONFIGUI_H
#define SCENARIOSCONFIGUI_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <unordered_map>

class EscenarioFile;

namespace Ui {
class ScenariosConfigUI;
}

class ScenariosConfigUI : public QWidget
{
		Q_OBJECT

	public:
		explicit ScenariosConfigUI(QWidget *parent = 0);
		~ScenariosConfigUI();
		void setScenarioFile(EscenarioFile* file);
		void addMaterialScenario(int, QString, QString abbr);
		void addSismicScenario(int, QString, QString abbr);

	public slots:
		void addNewMaterialScenario();
		void addNewSismicScenario();
		void removeMaterialScenario();
		void removeSismicScenario();
		void changedNameSismic(QString);
		void changedNameMaterial(QString);
		void changedAbbrSismic(QString);
		void changedAbbrMaterial(QString);


	signals:
		void removedMaterialScenario(int); // key
		void removedSismicScenario(int); // key
		void addedMaterialScenario(int, QString, QString); // key, name, abbr
		void addedSismicScenario(int, QString, QString); // key, name, abbr
		void changedNameMaterialScenario(int, QString); // key, name
		void changedNameSismicScenario(int, QString); // key, name
		void changedAbbrMaterialScenario(int, QString); // key, abbr
		void changedAbbrSismicScenario(int, QString); // key, abbr

	private:
		int scenarios_index;
		Ui::ScenariosConfigUI *ui;
		EscenarioFile * scenarios;
		std::unordered_map<int, QLineEdit*> line_edits_material;
		std::unordered_map<int, QLineEdit*> line_edits_abbr_material;
		std::unordered_map<QPushButton*, int> remove_buttons_material;
		std::unordered_map<QLineEdit*, int> line_edits_material_inv;
		std::unordered_map<int, QLineEdit*> line_edits_sismic;
		std::unordered_map<int, QLineEdit*> line_edits_abbr_sismic;
		std::unordered_map<QPushButton*, int> remove_buttons_sismic;
		std::unordered_map<QLineEdit*, int> line_edits_sismic_inv;
};

#endif // SCENARIOSCONFIGUI_H
