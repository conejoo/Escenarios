#ifndef SCENARIOSTRENGTHFUNCTIONCONFIG_H
#define SCENARIOSTRENGTHFUNCTIONCONFIG_H

#include <QWidget>
#include <unordered_map>

namespace Ui {
class ScenarioStrengthFunctionConfig;
}
class StrengthFunction;
class QSpinBox;
class QLabel;
namespace QtCharts {
class QPieSeries;
class QChartView;
}
class ScenarioStrengthFunctionConfig : public QWidget
{
		Q_OBJECT

	public:
		explicit ScenarioStrengthFunctionConfig(QWidget *parent = 0, StrengthFunction* function = 0);
		~ScenarioStrengthFunctionConfig();
		void toggleProperty(QString name, bool toggled);

	public slots:
		void changedAngles();
		void applyAngleShift();
		void setName(QString name);

	private:
		Ui::ScenarioStrengthFunctionConfig *ui;
		StrengthFunction* function;
		QSpinBox* getAngleSpinBox(int row);
		std::vector<QSpinBox*> createSpinBoxes(std::vector<int> &values_row);
		QWidget* createColorWidget(std::vector<int> &values_row);
		void setupPieChart(std::vector<std::vector<int> > &values_row);
		void moveRow(int row, int target);
		void removeRow(int row);
		void updateAngleConstraints();
		void readValuesFromSpinBoxes();
		void rebuildGridLayoutSpinBoxes();
		static int getPropertyIndex(QString name);
		bool collapseSlices();
		QtCharts::QPieSeries *series;
		QtCharts::QChartView *chartView;
		QBrush getColor(std::vector<int>& values);

		std::vector<std::vector<int> >& original_values;
		std::vector<std::vector<int> > updated_values;
		std::vector<std::vector<QSpinBox*> > spin_boxes;
		std::vector<QWidget*> color_widgets;
		std::vector<QLabel*> spin_box_labels;
		std::vector<QBrush> colors;
		std::vector<bool> property_toggle_state;
		std::unordered_map<std::string, QBrush> color_map;
		int next_color;
};

#endif // SCENARIOSTRENGTHFUNCTIONCONFIG_H
