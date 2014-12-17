#ifndef SCENARIOMATERIALSCONFIGUI_H
#define SCENARIOMATERIALSCONFIGUI_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <vector>
#include "material.h"

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
		void toggleProperty(int index, bool toggled);
		void applyPercentaje(double percentaje, int property_index);

	public slots:
		void updatePropertyValue(double new_value);

	private:
		Ui::ScenarioMaterialsConfigUI *ui;
		Material& material;
		int material_index;
		std::unordered_map<QDoubleSpinBox*, MaterialProperty*> line_edits_material;
		std::unordered_map<MaterialProperty*, QDoubleSpinBox*> property_line_edit;
};

#endif // SCENARIOMATERIALSCONFIGUI_H
