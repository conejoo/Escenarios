#ifndef MATERIALESCENARIOSGENERALCONFIG_H
#define MATERIALESCENARIOSGENERALCONFIG_H

#include <QWidget>
#include <unordered_map>
#include "material.h"

class QPushButton;
class QDoubleSpinBox;

namespace Ui {
class MaterialEscenariosGeneralConfig;
}

class MaterialEscenariosGeneralConfig : public QWidget
{
		Q_OBJECT

	public:
		explicit MaterialEscenariosGeneralConfig(QWidget *parent, Material &material, int index);
		~MaterialEscenariosGeneralConfig();
		void setName(QString);
		void toggleProperty(int index, bool toggled);

	public slots:
		void applyPercentaje();

	signals:
		void percentajeApplied(double percent, int scenario_index, int property_index);

	private:
		Ui::MaterialEscenariosGeneralConfig *ui;
		int row;
		int scenario_index;
		std::unordered_map<QPushButton*, std::pair<int, QDoubleSpinBox*> > buttons_map;
};

#endif // MATERIALESCENARIOSGENERALCONFIG_H
