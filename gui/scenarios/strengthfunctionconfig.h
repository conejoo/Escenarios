#ifndef STRENGTHFUNCTIONCONFIG_H
#define STRENGTHFUNCTIONCONFIG_H

#include <QWidget>
#include <unordered_map>

namespace Ui {
class StrengthFunctionConfig;
}
class StrengthFunction;
class QSpinBox;
class QLabel;
namespace QtCharts {
class QPieSeries;
class QChartView;
}
class StrengthFunctionConfig : public QWidget
{
		Q_OBJECT

	public:
		explicit StrengthFunctionConfig(QWidget *parent = 0, StrengthFunction* function = 0);
		~StrengthFunctionConfig();

	public slots:
		void changedAngles();
		void applyAngleShift();

	private:
		Ui::StrengthFunctionConfig *ui;
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
		std::unordered_map<std::string, QBrush> color_map;
		int next_color;
};

#endif // STRENGTHFUNCTIONCONFIG_H
