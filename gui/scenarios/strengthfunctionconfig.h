#ifndef STRENGTHFUNCTIONCONFIG_H
#define STRENGTHFUNCTIONCONFIG_H

#include <unordered_map>
#include <QWidget>


class StrengthFunction;

namespace Ui {
class StrengthFunctionConfig;
}

class ScenarioStrengthFunctionConfig;

class StrengthFunctionConfig : public QWidget
{
		Q_OBJECT

	public:
		explicit StrengthFunctionConfig(QWidget *parent, StrengthFunction* strength_function);
		~StrengthFunctionConfig();
		void escenarioRemoved(int index);
		void escenarioAdded(int index, QString name);
		void escenarioChangedName(int index, QString newname);
		void applyPercentaje(double percentaje, int scenario_index, int property_index);
		void toggleScenario(int index, bool show);
		void toggleProperty(QString name, bool toggled);

	private:
		Ui::StrengthFunctionConfig *ui;
		StrengthFunction* strength_function;
		std::unordered_map<int, ScenarioStrengthFunctionConfig*> strength_function_scenarios;
};

#endif // STRENGTHFUNCTIONCONFIG_H
