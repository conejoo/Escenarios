#include "qspinboxwithdata.h"

QSpinBoxWithData::QSpinBoxWithData(QWidget *parent)
	: QSpinBox(parent)
{
}
int QSpinBoxWithData::getData(int key)
{
	return data[key];
}
void QSpinBoxWithData::putData(int key, int value)
{
	data[key] = value;
}
QSpinBoxWithData* QSpinBoxWithData::chainSetValue(int value)
{
	this->setValue(value);
	return this;
}
QSpinBoxWithData* QSpinBoxWithData::chainSetRange(int min, int max)
{
	this->setRange(min, max);
	return this;
}
QSpinBoxWithData* QSpinBoxWithData::chainBlockSignals(bool block)
{
	this->blockSignals(block);
	return this;
}
void QSpinBoxWithData::setValueBlockingSignals(int value)
{
	this->blockSignals(true);
	this->setValue(value);
	this->blockSignals(false);
}
void QSpinBoxWithData::setRangeBlockingSignals(int min, int max)
{
	this->blockSignals(true);
	this->setRange(min, max);
	this->blockSignals(false);
}
