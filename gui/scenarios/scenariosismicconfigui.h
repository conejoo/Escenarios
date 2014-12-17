#ifndef SCENARIOSISMICCONFIGUI_H
#define SCENARIOSISMICCONFIGUI_H

#include <QWidget>

#include "model/scenarios/escenarioseismiccustom.h"

namespace Ui {
class ScenarioSismicConfigUI;
}

class ScenarioSismicConfigUI : public QWidget
{
		Q_OBJECT

	public:
		explicit ScenarioSismicConfigUI(QWidget *parent, EscenarioSeismicCustom* scenario);
		virtual ~ScenarioSismicConfigUI();
		void setNewName(QString, QString);

	public slots:
		void changedSeismic();
		void toggled(bool);

	private:
		Ui::ScenarioSismicConfigUI *ui;
		EscenarioSeismicCustom* scenario;
};

#endif // SCENARIOSISMICCONFIGUI_H
