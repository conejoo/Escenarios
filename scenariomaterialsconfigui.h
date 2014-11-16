#ifndef SCENARIOMATERIALSCONFIGUI_H
#define SCENARIOMATERIALSCONFIGUI_H

#include <QWidget>
#include <vector>
#include "material.h"

namespace Ui {
class ScenarioMaterialsConfigUI;
}

class ScenarioMaterialsConfigUI : public QWidget
{
		Q_OBJECT

	public:
		explicit ScenarioMaterialsConfigUI(QWidget *parent, std::vector<Material>& materials, int index);
		~ScenarioMaterialsConfigUI();

	private:
		Ui::ScenarioMaterialsConfigUI *ui;
		std::vector<Material>& materials;
		bool read_only;
		int index;
};

#endif // SCENARIOMATERIALSCONFIGUI_H
