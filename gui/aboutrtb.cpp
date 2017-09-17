#include "aboutrtb.h"
#include "ui_aboutrtb.h"

AboutRTB::AboutRTB(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AboutRTB)
{
	ui->setupUi(this);
}

AboutRTB::~AboutRTB()
{
	delete ui;
}
