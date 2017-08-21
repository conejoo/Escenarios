#include "strengthfunctionconfig.h"
#include "ui_strengthfunctionconfig.h"
#include "model/scenarios/strengthfunction.h"

#include <iostream>

#include <QSpinBox>
#include <QLabel>
#include <QPalette>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

StrengthFunctionConfig::StrengthFunctionConfig(QWidget *parent, StrengthFunction *function) :
	QWidget(parent),
	ui(new Ui::StrengthFunctionConfig),
	original_values(function->values),
	updated_values(function->values)
{
	ui->setupUi(this);
	this->function = function;
	this->next_color = 0;
	ui->groupBox->setTitle(QString::fromStdString(function->name + (function->in_use ? " (Usada)" : "")));

	colors.push_back(Qt::green);
	colors.push_back(Qt::blue);
	colors.push_back(Qt::cyan);
	colors.push_back(Qt::yellow);
	colors.push_back(Qt::magenta);
	colors.push_back(Qt::red);
	colors.push_back(Qt::gray);

	this->series = new QtCharts::QPieSeries();
	QtCharts::QChart *chart = new QtCharts::QChart();
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

StrengthFunctionConfig::~StrengthFunctionConfig()
{
	delete ui;
}

void StrengthFunctionConfig::setupPieChart(std::vector<std::vector<int> > &values_row) {
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

QWidget* StrengthFunctionConfig::createColorWidget(std::vector<int> &values_row) {
	QWidget *widget = new QWidget();
	widget->setMinimumWidth(50);
	QPalette pal;
	// set black background
	pal.setColor(QPalette::ColorRole::Background, this->getColor(values_row).color());
	widget->setAutoFillBackground(true);
	widget->setPalette(pal);
	return widget;
}

std::vector<QSpinBox *> StrengthFunctionConfig::createSpinBoxes(std::vector<int> &values_row) {
	std::vector<QSpinBox*> row_spin_boxes;
	for (unsigned int i = 0; i < values_row.size(); i++) {
		QSpinBox* line_edit = new QSpinBox();
		line_edit->setRange(-100000, 100000);
		line_edit->setValue(values_row[i]);
		if (i == 0) {
			connect(line_edit, SIGNAL(valueChanged(int)), this, SLOT(changedAngles()));
			line_edit->setSuffix(QString(" °"));
		}
		row_spin_boxes.push_back(line_edit);
	}
	return row_spin_boxes;
}
QBrush StrengthFunctionConfig::getColor(std::vector<int>& values) {
	std::string key = QStringLiteral("serie_%1_%1").arg(values[1], values[2]).toStdString();
	if (this->color_map.find(key) == this->color_map.end()) {
		// not found
		this->color_map[key] = this->colors[next_color];
		this->next_color = (this->next_color + 1) % this->colors.size();
	}
	return this->color_map[key];
}

void StrengthFunctionConfig::changedAngles() {
	this->updateAngleConstraints();
	this->setupPieChart(this->updated_values);
	std::cout << "Update angle constraints!" << std::endl;
}

void StrengthFunctionConfig::readValuesFromSpinBoxes() {
	for (unsigned int row = 0; row < spin_boxes.size(); ++row) {
		for (unsigned int col = 0; col < spin_boxes[row].size(); ++col) {
			updated_values[row][col] = spin_boxes[row][col]->value();
		}
	}
}

void StrengthFunctionConfig::rebuildGridLayoutSpinBoxes() {
	QGridLayout *grid_layout = ui->valueGrid;
	QLayoutItem *child;
	std::vector<QString> fields = {"ang", "c", "phi"};
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

bool StrengthFunctionConfig::collapseSlices() {
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

void StrengthFunctionConfig::moveRow(int row, int target) {
	std::vector<int> aux_int = updated_values[row];
	std::vector<QSpinBox*> aux_spin_boxes = spin_boxes[row];
	QWidget* aux_color_widget = this->color_widgets[row];
	updated_values.erase(updated_values.begin() + row);
	updated_values.insert(updated_values.begin() + target, aux_int);
	spin_boxes.erase(spin_boxes.begin() + row);
	spin_boxes.insert(spin_boxes.begin() + target, aux_spin_boxes);
	color_widgets.erase(color_widgets.begin() + row);
	color_widgets.insert(color_widgets.begin() + target, aux_color_widget);
}

void StrengthFunctionConfig::removeRow(int row) {
	updated_values.erase(updated_values.begin() + row);
	for (QSpinBox* sp: spin_boxes[row])
		delete sp;
	spin_boxes.erase(spin_boxes.begin() + row);
	delete color_widgets[row];
	color_widgets.erase(color_widgets.begin() + row);
}

void StrengthFunctionConfig::applyAngleShift() {
	this->readValuesFromSpinBoxes();
	int shift_value = ui->angleShiftSpinBox->value();
	std::cout << "angleShiftSpinBox: " << shift_value << std::endl;
	for (std::vector<QSpinBox*>& sp: spin_boxes) {
		sp[0]->blockSignals(true);
		sp[0]->setRange(-100000, 100000);
		sp[0]->setValue(sp[0]->value() + shift_value);
		sp[0]->blockSignals(false);
		std::cout << "New SpinBox Value: " << sp[0]->value() << std::endl;
	}
	bool need_rebuild = false;
	if (shift_value > 0) {
		while(true) {
			int row = spin_boxes.size() - 1;
			int end_value = spin_boxes[row][0]->value();
			if (end_value > 90) {
				spin_boxes[row][0]->blockSignals(true);
				spin_boxes[row][0]->setValue(90);
				spin_boxes[row][0]->blockSignals(false);
				int new_value = end_value - 180;
				updated_values.insert(updated_values.begin(), updated_values[row]);
				updated_values[0][0] = new_value; // angle
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
				spin_boxes[row][0]->blockSignals(true);
				spin_boxes[row][0]->setValue(-90);
				spin_boxes[row][0]->blockSignals(false);
				int new_value = end_value + 180;
				updated_values.insert(updated_values.end(), updated_values[row]);
				updated_values[updated_values.size() - 1][0] = new_value; // angle
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
					std::vector<int> new_bottom = {90, updated_values[0][1], updated_values[0][2]};
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
			std::vector<int> new_end = {std::min(90, updated_values[updated_values.size() - 1][0]) - shift_value, updated_values[0][1], updated_values[0][2]};
			updated_values.insert(updated_values.end(), new_end);
			spin_boxes.insert(spin_boxes.end(), this->createSpinBoxes(new_end));
			color_widgets.insert(color_widgets.end(), this->createColorWidget(new_end));
		}
		while(this->collapseSlices());
		this->rebuildGridLayoutSpinBoxes();
	}
	this->changedAngles();
}

QSpinBox* StrengthFunctionConfig::getAngleSpinBox(int row) {
	QGridLayout * grid_layout = ui->valueGrid;
	QLayoutItem * item = grid_layout->itemAtPosition(row, 1);
	return qobject_cast<QSpinBox *>(item->widget());
}

void StrengthFunctionConfig::updateAngleConstraints() {
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

