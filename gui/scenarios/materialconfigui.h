#ifndef MATERIALCONFIGUI_H
#define MATERIALCONFIGUI_H

#include <QWidget>
#include <unordered_map>

#include "model/scenarios/material.h"
#include "gui/scenarios/scenariomaterialsconfigui.h"

namespace Ui {
class MaterialConfigUI;
}

class MaterialConfigUI : public QWidget
{
		Q_OBJECT

	public:
		explicit MaterialConfigUI(QWidget *parent, Material& material);
		~MaterialConfigUI();
		void escenarioRemoved(int index);
		void escenarioAdded(int index, QString name);
		void escenarioChangedName(int index, QString newname);
		void toggleMaterial(int index, bool show);
		void toggleProperty(int index, bool toggled);
		void applyPercentaje(double percentaje, int scenario_index, int property_index);

	private:
		Ui::MaterialConfigUI *ui;
		Material& material;
		std::unordered_map<int, ScenarioMaterialsConfigUI*> material_scenarios;
};

#endif // MATERIALCONFIGUI_H
