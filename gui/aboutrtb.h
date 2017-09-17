#ifndef ABOUTRTB_H
#define ABOUTRTB_H

#include <QWidget>

namespace Ui {
class AboutRTB;
}

class AboutRTB : public QWidget
{
		Q_OBJECT

	public:
		explicit AboutRTB(QWidget *parent = 0);
		~AboutRTB();

	private:
		Ui::AboutRTB *ui;
};

#endif // ABOUTRTB_H
