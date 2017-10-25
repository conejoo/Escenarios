#ifndef SCENARIOSTRENGTHFUNCTIONCONFIG_H
#define SCENARIOSTRENGTHFUNCTIONCONFIG_H

#include <QWidget>
#include <unordered_map>
#include "model/scenarios/strengthfunctionpieslice.h"

namespace Ui {
class ScenarioStrengthFunctionConfig;
}
class StrengthFunction;
class QSpinBoxWithData;
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
		void applyPercentaje(double percentaje, QString property_short_name, std::vector<StrengthFunctionPieSlice> base_values);
		void applyAngleShift(int angle);
		std::vector<StrengthFunctionPieSlice> getCurrentValues();

	public slots:
		void changedAngles();
		void changedOtherValue();
		void applyAngleShift();
		void setName(QString name);

	private:
		Ui::ScenarioStrengthFunctionConfig *ui;
		StrengthFunction* function;
		QSpinBoxWithData* getAngleSpinBox(int row);
		std::vector<QSpinBoxWithData*> createSpinBoxes(StrengthFunctionPieSlice &values_row);
		QWidget* createColorWidget(StrengthFunctionPieSlice &values_row);
		void updateColorWidgets(std::vector<StrengthFunctionPieSlice> &values_row);
		void setupPieChart(std::vector<StrengthFunctionPieSlice> &values_row);
		void moveRow(int row, int target);
		void removeRow(int row);
		void updateAngleConstraints();
		void readValuesFromSpinBoxes();
		void rebuildGridLayoutSpinBoxes();
		bool collapseSlices();

		static int getPropertyIndex(QString name);
		static const int PIE_SLICE_MATERIAL_KEY = 1;
		static const int PIE_SLICE_PROPERTY_INDEX = 2;

		QtCharts::QPieSeries *series;
		QtCharts::QChartView *chartView;
		QBrush getColor(StrengthFunctionPieSlice &values);

		std::vector<StrengthFunctionPieSlice>& original_values;
		std::vector<StrengthFunctionPieSlice> updated_values;
		std::vector<std::vector<QSpinBoxWithData*> > spin_boxes;
		std::vector<QWidget*> color_widgets;
		std::vector<QLabel*> spin_box_labels;
		std::vector<QBrush> colors;
		std::vector<bool> property_toggle_state;
		std::unordered_map<std::string, QBrush> color_map;
		int next_color;
};

#endif // SCENARIOSTRENGTHFUNCTIONCONFIG_H
