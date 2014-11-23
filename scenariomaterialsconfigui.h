#ifndef SCENARIOMATERIALSCONFIGUI_H
#define SCENARIOMATERIALSCONFIGUI_H

#include <QWidget>
#include <QLineEdit>
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

	public slots:
		void updatePropertyValue(QString new_value);

	private:
		Ui::ScenarioMaterialsConfigUI *ui;
		Material& material;
		int material_index;
		std::unordered_map<QLineEdit*, MaterialProperty*> line_edits_material;
};

#endif // SCENARIOMATERIALSCONFIGUI_H
