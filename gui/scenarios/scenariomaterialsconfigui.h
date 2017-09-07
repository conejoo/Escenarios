#ifndef SCENARIOMATERIALSCONFIGUI_H
#define SCENARIOMATERIALSCONFIGUI_H

#include <QWidget>
#include <vector>

#include "model/scenarios/material.h"

class QDoubleSpinBox;
class QComboBox;

namespace Ui {
class ScenarioMaterialsConfigUI;
}

class ScenarioMaterialsConfigUI : public QWidget
{
		Q_OBJECT

	public:
		explicit ScenarioMaterialsConfigUI(QWidget *parent, int index, Material &material);
		~ScenarioMaterialsConfigUI();
		void setName(QString);
		void toggleProperty(QString name, bool toggled);
		void applyPercentaje(double percentaje, int property_index);
		//void setupStrengthsFunctionsCombobox(QStringList& list);

	public slots:
		void updatePropertyValue(double new_value);

	private:
		Ui::ScenarioMaterialsConfigUI *ui;
		Material& material;
		int material_index;
		std::unordered_map<QDoubleSpinBox*, MaterialProperty*> line_edits_material;
		std::unordered_map<MaterialProperty*, QDoubleSpinBox*> property_line_edit;
		QHash<QString, int> property_name_index;
		QComboBox *str_functions;
};

#endif // SCENARIOMATERIALSCONFIGUI_H
