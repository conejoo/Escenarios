#ifndef MATERIALGENERALCONFIG_H
#define MATERIALGENERALCONFIG_H

#include <QWidget>
#include <unordered_map>
#include "model/scenarios/material.h"

class MaterialEscenariosGeneralConfig;
class EscenarioMaterialCustom;

namespace Ui {
class MaterialGeneralConfig;
}

class MaterialGeneralConfig : public QWidget
{
		Q_OBJECT

	public:
		explicit MaterialGeneralConfig(QWidget *parent = 0);
		~MaterialGeneralConfig();
		void clearScenarios();
		void addScenario(EscenarioMaterialCustom*, Material& m);
		void escenarioRemoved(int index);
		void escenarioChangedName(int index, QString newname);
		void toggleMaterial(int index, bool show);
		void toggleProperty(QString name, bool toggled);

	signals:
		void percentajeApplied(double percent, int scenario_index, int property_index);

	private:
		std::unordered_map<int, MaterialEscenariosGeneralConfig*> configs;
		Ui::MaterialGeneralConfig *ui;
};

#endif // MATERIALGENERALCONFIG_H
