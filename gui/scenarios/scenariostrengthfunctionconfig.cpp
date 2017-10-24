#include "scenariostrengthfunctionconfig.h"
#include "ui_scenariostrengthfunctionconfig.h"
#include "model/scenarios/strengthfunction.h"
#include "gui/utils/qspinboxwithdata.h"

#include <iostream>

#include <QLabel>
#include <QPalette>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGraphicsLayout>

ScenarioStrengthFunctionConfig::ScenarioStrengthFunctionConfig(QWidget *parent, StrengthFunction *function) :
	QWidget(parent),
	ui(new Ui::ScenarioStrengthFunctionConfig),
	original_values(function->values),
	updated_values(function->values)
{
	ui->setupUi(this);
	this->function = function;
	this->next_color = 0;
	ui->groupBox->setTitle(QString::fromStdString(function->name + (function->in_use ? " (Usada)" : "")));

	property_toggle_state.push_back(true);
	property_toggle_state.push_back(true);
	property_toggle_state.push_back(true);

	colors.push_back(Qt::green);
	colors.push_back(Qt::blue);
	colors.push_back(Qt::cyan);
	colors.push_back(Qt::yellow);
	colors.push_back(Qt::magenta);
	colors.push_back(Qt::red);
	colors.push_back(Qt::gray);

	this->series = new QtCharts::QPieSeries();
	QtCharts::QChart *chart = new QtCharts::QChart();
	chart->layout()->setContentsMargins(0, 0, 0, 0);
	//chart->setBackgroundVisible(false);
	chart->setMargins(QMargins());
	chart->setMinimumHeight(100);
	chart->setPlotAreaBackgroundVisible(false);
	chart->addSeries(this->series);
	//chart->setTitle("Simple piechart example");
	chart->legend()->hide();
	chartView = new QtCharts::QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	QLayout * groupLayout = ui->groupBox->layout();
	groupLayout->addWidget(chartView);

	for(int row = 0; row < (int) function->values.size(); row++) {
		spin_boxes.push_back(this->createSpinBoxes(function->values[row]));
		color_widgets.push_back(this->createColorWidget(function->values[row]));

	}
	this->rebuildGridLayoutSpinBoxes();
	this->setupPieChart(function->values);
	connect(ui->applyAngleShiftButton, SIGNAL(clicked(bool)), this, SLOT(applyAngleShift()));
}

ScenarioStrengthFunctionConfig::~ScenarioStrengthFunctionConfig()
{
	delete ui;
}

void ScenarioStrengthFunctionConfig::setupPieChart(std::vector<StrengthFunctionPieSlice> &values_row) {
	series->clear();
	for (unsigned int i = 0; i < values_row.size(); i++) {
		int value = spin_boxes[i][0]->value() + 90;
		if (i > 0)
			value -= spin_boxes[i - 1][0]->value() + 90;
		//std::cout << "Values pie (" << i << "): " << value << std::endl;
		QtCharts::QPieSlice *slice = new QtCharts::QPieSlice(QStringLiteral("Serie %1").arg(i), value);
		slice->setBrush(this->getColor(values_row[i]));
		series->append(slice);
	}
	QtCharts::QPieSlice *whiteSlice = new QtCharts::QPieSlice("Filler", 180);
	whiteSlice->setBrush(Qt::white);
	series->append(whiteSlice);


//	QtCharts::QPieSlice *slice = series->slices().at(1);
//	slice->setExploded();
//	slice->setLabelVisible();
//	slice->setPen(QPen(Qt::darkGreen, 2));
//	slice->setBrush(Qt::green);
//	chartView->repaint();
}

void ScenarioStrengthFunctionConfig::updateColorWidgets(std::vector<StrengthFunctionPieSlice> &values_row) {
	for (unsigned int row = 0; row < values_row.size(); row++) {
		QPalette pal;
		pal.setColor(QPalette::ColorRole::Background, this->getColor(values_row[row]).color());
		color_widgets[row]->setPalette(pal);
	}
}

QWidget* ScenarioStrengthFunctionConfig::createColorWidget(StrengthFunctionPieSlice &values_row) {
	QWidget *widget = new QWidget();
	widget->setMinimumWidth(30);
	QPalette pal;
	// set black background
	pal.setColor(QPalette::ColorRole::Background, this->getColor(values_row).color());
	widget->setAutoFillBackground(true);
	widget->setPalette(pal);
	return widget;
}

std::vector<QSpinBoxWithData *> ScenarioStrengthFunctionConfig::createSpinBoxes(StrengthFunctionPieSlice &values_row) {
	std::vector<QSpinBoxWithData*> row_spin_boxes;
	for (unsigned int i = 0; i < 3; i++) {
		QSpinBoxWithData* line_edit = new QSpinBoxWithData();
		line_edit->putData(PIE_SLICE_MATERIAL_KEY, values_row.material_index);
		line_edit->putData(PIE_SLICE_PROPERTY_INDEX, i);
		line_edit->setRange(-100000, 100000);
		line_edit->setValue(values_row.getValue(i));
		if (i == 0) {
			connect(line_edit, SIGNAL(valueChanged(int)), this, SLOT(changedAngles()));
			line_edit->setSuffix(QString(" °"));
		}
		else {
			connect(line_edit, SIGNAL(valueChanged(int)), this, SLOT(changedOtherValue()));
		}
		line_edit->setEnabled(property_toggle_state[i]);
		row_spin_boxes.push_back(line_edit);
	}
	return row_spin_boxes;
}
QBrush ScenarioStrengthFunctionConfig::getColor(StrengthFunctionPieSlice& values) {
	std::string key = QStringLiteral("serie_%1_%1").arg(values.cohesion, values.phi).toStdString();
	if (this->color_map.find(key) == this->color_map.end()) {
		// not found
		this->color_map[key] = this->colors[next_color];
		this->next_color = (this->next_color + 1) % this->colors.size();
	}
	return this->color_map[key];
}

void ScenarioStrengthFunctionConfig::changedOtherValue() {
	QSpinBoxWithData * qspinbox = qobject_cast<QSpinBoxWithData *>(sender());
	int pie_material_index = qspinbox->getData(PIE_SLICE_MATERIAL_KEY);
	int pie_property_index = qspinbox->getData(PIE_SLICE_PROPERTY_INDEX);
	for (std::vector<QSpinBoxWithData*>& row: spin_boxes) {
		if(row[pie_property_index] != qspinbox &&
				row[pie_property_index]->getData(PIE_SLICE_MATERIAL_KEY) == pie_material_index) {
			row[pie_property_index]->setValueBlockingSignals(qspinbox->value());
		}
	}
}

void ScenarioStrengthFunctionConfig::changedAngles() {
	this->updateAngleConstraints();
	this->setupPieChart(this->updated_values);
	this->updateColorWidgets(this->updated_values);
	std::cout << "Update angle constraints!" << std::endl;
}

void ScenarioStrengthFunctionConfig::readValuesFromSpinBoxes() {
	for (unsigned int row = 0; row < spin_boxes.size(); ++row) {
		updated_values[row].angle = spin_boxes[row][0]->value();
		updated_values[row].cohesion = spin_boxes[row][1]->value();
		updated_values[row].phi = spin_boxes[row][2]->value();
//		for (unsigned int col = 0; col < spin_boxes[row].size(); ++col) {
//			updated_values[row].setValue(col, spin_boxes[row][col]->value());
//		}
	}
}

void ScenarioStrengthFunctionConfig::rebuildGridLayoutSpinBoxes() {
	QGridLayout *grid_layout = ui->valueGrid;
	QLayoutItem *child;
	std::vector<QString> fields = {"Angle", "Cohesion", "Phi"};
	while ((child = grid_layout->takeAt(0)) != 0);
	for (QLabel *label: spin_box_labels)
		delete label;
	spin_box_labels.clear();
	for (unsigned int row = 0; row < spin_boxes.size(); ++row) {
		for (unsigned int col = 0; col < spin_boxes[row].size(); ++col) {
			QLabel* label = new QLabel(fields[col]);
			spin_box_labels.push_back(label);
			grid_layout->addWidget(label, row, col * 2);
			grid_layout->addWidget(spin_boxes[row][col], row, col * 2 + 1);
		}
		grid_layout->addWidget(color_widgets[row], row, spin_boxes[row].size() * 2);
	}
}

bool ScenarioStrengthFunctionConfig::collapseSlices() {
	// Try collapse end
	for (unsigned int row = 1; row < spin_boxes.size(); ++row) {
		if (spin_boxes[row - 1][1]->value() == spin_boxes[row][1]->value() &&
				spin_boxes[row - 1][2]->value() == spin_boxes[row][2]->value()) {
			std::cout << "Collapsing row: " << row << std::endl;
			this->removeRow(row - 1);
			return true;
		}
	}
	return false;
}

void ScenarioStrengthFunctionConfig::moveRow(int row, int target) {
	StrengthFunctionPieSlice aux_int = updated_values[row];
	std::vector<QSpinBoxWithData*> aux_spin_boxes = spin_boxes[row];
	QWidget* aux_color_widget = this->color_widgets[row];
	updated_values.erase(updated_values.begin() + row);
	updated_values.insert(updated_values.begin() + target, aux_int);
	spin_boxes.erase(spin_boxes.begin() + row);
	spin_boxes.insert(spin_boxes.begin() + target, aux_spin_boxes);
	color_widgets.erase(color_widgets.begin() + row);
	color_widgets.insert(color_widgets.begin() + target, aux_color_widget);
}

void ScenarioStrengthFunctionConfig::removeRow(int row) {
	updated_values.erase(updated_values.begin() + row);
	for (QSpinBox* sp: spin_boxes[row])
		delete sp;
	spin_boxes.erase(spin_boxes.begin() + row);
	delete color_widgets[row];
	color_widgets.erase(color_widgets.begin() + row);
}

void ScenarioStrengthFunctionConfig::applyAngleShift() {
	int shift_value = ui->angleShiftSpinBox->value();
	this->applyAngleShift(shift_value);
}

void ScenarioStrengthFunctionConfig::applyAngleShift(int shift_value) {
	this->readValuesFromSpinBoxes();
	std::cout << "angleShiftSpinBox: " << shift_value << std::endl;
	for (std::vector<QSpinBoxWithData*>& sp: spin_boxes) {
		sp[0]->setRangeBlockingSignals(-100000, 100000);
		sp[0]->setValueBlockingSignals(sp[0]->value() + shift_value);
		std::cout << "New SpinBox Value: " << sp[0]->value() << std::endl;
	}
	bool need_rebuild = false;
	if (shift_value > 0) {
		while(true) {
			int row = spin_boxes.size() - 1;
			int end_value = spin_boxes[row][0]->value();
			if (end_value > 90) {
				spin_boxes[row][0]->setValueBlockingSignals(90);
				int new_value = end_value - 180;
				updated_values.insert(updated_values.begin(), updated_values[row]);
				updated_values[0].angle = new_value; // angle
				spin_boxes.insert(spin_boxes.begin(), this->createSpinBoxes(updated_values[0]));
				color_widgets.insert(color_widgets.begin(), this->createColorWidget(updated_values[0]));
				// Check if we have to remove the last slice
				need_rebuild = true;
				++row; // just inserted new row
				if (row - 1 >= 0 && spin_boxes[row - 1][0]->value() >= 90) {
					// Remove last slice
					this->removeRow(row);
				}
				else
					break;

			}
			else
				break;
		}
	}
	if (shift_value < 0) {
		while(true) {
			int row = 0;
			int end_value = spin_boxes[row][0]->value();
			if (end_value < -90) {
				spin_boxes[row][0]->setValueBlockingSignals(-90);
				int new_value = end_value + 180;
				updated_values.insert(updated_values.end(), updated_values[row]);
				updated_values[updated_values.size() - 1].angle = new_value; // angle
				spin_boxes.insert(spin_boxes.end(), this->createSpinBoxes(updated_values[updated_values.size() - 1]));
				color_widgets.insert(color_widgets.end(), this->createColorWidget(updated_values[updated_values.size() - 1]));
				// Check if we have to remove the last slice
				need_rebuild = true;
				if (spin_boxes[0][0]->value() <= -90) {
					// Remove last slice
					this->removeRow(0);
				}
				else
					break;

			}
			else {
				if (!need_rebuild) {
					// Move fraction to the bottom of the size of the shift
					StrengthFunctionPieSlice new_bottom(updated_values[0]);
					new_bottom.angle = 90;
					updated_values.insert(updated_values.end(), new_bottom);
					spin_boxes.insert(spin_boxes.end(), this->createSpinBoxes(new_bottom));
					color_widgets.insert(color_widgets.end(), this->createColorWidget(new_bottom));
					need_rebuild = true;
				}
				break;
			}
		}
	}
	if (need_rebuild) {
		if (shift_value < 0 && spin_boxes[spin_boxes.size() - 1][0]->value() < 90) {
			StrengthFunctionPieSlice new_end(updated_values[0]);
			new_end.angle = std::min(90, updated_values[updated_values.size() - 1].angle) - shift_value;
			updated_values.insert(updated_values.end(), new_end);
			spin_boxes.insert(spin_boxes.end(), this->createSpinBoxes(new_end));
			color_widgets.insert(color_widgets.end(), this->createColorWidget(new_end));
		}
		while(this->collapseSlices());
		this->rebuildGridLayoutSpinBoxes();
	}
	this->changedAngles();
}

QSpinBoxWithData *ScenarioStrengthFunctionConfig::getAngleSpinBox(int row) {
	QGridLayout * grid_layout = ui->valueGrid;
	QLayoutItem * item = grid_layout->itemAtPosition(row, 1);
	return qobject_cast<QSpinBoxWithData *>(item->widget());
}

void ScenarioStrengthFunctionConfig::updateAngleConstraints() {
	for (unsigned int row = 0; row < spin_boxes.size(); ++row) {
		std::cout << "Setting for row: "<< row << std::endl;
		QSpinBox* line_edit = spin_boxes[row][0];
		int min = -90;
		int max = 90;
		if (row > 0) {
			QSpinBox* prev_line_edit = spin_boxes[row - 1][0];
			min = prev_line_edit->value();
		}
		if (row + 1 < spin_boxes.size()) {
			QSpinBox* next_line_edit = spin_boxes[row + 1][0];
			max = next_line_edit->value();
		}
		line_edit->setRange(min, max);
	}
}

void ScenarioStrengthFunctionConfig::setName(QString name) {
	ui->groupBox->setTitle(name);
}

int ScenarioStrengthFunctionConfig::getPropertyIndex(QString name) {
	return StrengthFunction::getPropertyIndex(name.toStdString());
}

void ScenarioStrengthFunctionConfig::toggleProperty(QString name, bool toggled) {
	int index = ScenarioStrengthFunctionConfig::getPropertyIndex(name);
	if (index != -1) {
		for (std::vector<QSpinBoxWithData*>& sp: spin_boxes)
			sp[index]->setEnabled(toggled);
		property_toggle_state[index] = toggled;
		if (index == 0) {
			// Angle
			ui->angleShiftSpinBox->setEnabled(toggled);
			ui->applyAngleShiftButton->setEnabled(toggled);
		}
	}
}

void ScenarioStrengthFunctionConfig::applyPercentaje(double percentaje, QString property_short_name) {
	int index = ScenarioStrengthFunctionConfig::getPropertyIndex(property_short_name);
	if (index != -1) {
		if (index == 0) {
			// Add angle
			this->applyAngleShift((int) percentaje);
		}
		else {
			// Percentaje
			for (std::vector<QSpinBoxWithData*>& sp: spin_boxes) {
				int new_value = sp[index]->value();
				new_value += (int) new_value * (percentaje / 100.0);
				sp[index]->setValue(new_value);
			}
		}
	}
}

std::vector<StrengthFunctionPieSlice> ScenarioStrengthFunctionConfig::getCurrentValues() {
	this->readValuesFromSpinBoxes();
	return updated_values;
}
