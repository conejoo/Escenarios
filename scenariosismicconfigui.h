#ifndef SCENARIOSISMICCONFIGUI_H
#define SCENARIOSISMICCONFIGUI_H

#include <QWidget>
#include "escenariofile.h"

namespace Ui {
class ScenarioSismicConfigUI;
}

class ScenarioSismicConfigUI : public QWidget
{
		Q_OBJECT

	public:
		explicit ScenarioSismicConfigUI(QWidget *parent, EscenarioFile& scenario, int index);
		virtual ~ScenarioSismicConfigUI();

	private:
		Ui::ScenarioSismicConfigUI *ui;
		EscenarioFile& scenario;
		int index;
};

#endif // SCENARIOSISMICCONFIGUI_H
