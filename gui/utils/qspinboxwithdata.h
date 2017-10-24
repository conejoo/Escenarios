#ifndef QSPINBOXWITHDATA_H
#define QSPINBOXWITHDATA_H

#include <QSpinBox>
#include <unordered_map>

class QSpinBoxWithData : public QSpinBox
{
		Q_OBJECT

	public:
		explicit QSpinBoxWithData(QWidget *parent = nullptr);
		int getData(int key);
		void putData(int key, int value);
		void setValueBlockingSignals(int value);
		void setRangeBlockingSignals(int min, int max);
		QSpinBoxWithData* chainSetValue(int value);
		QSpinBoxWithData* chainBlockSignals(bool block);
		QSpinBoxWithData* chainSetRange(int min, int max);
	private:
		std::unordered_map<int, int> data;

};

#endif // QSPINBOXWITHDATA_H
