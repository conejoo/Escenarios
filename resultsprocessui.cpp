#include "resultsprocessui.h"
#include "ui_resultsprocessui.h"
#include <QFileDialog>
#include <QLabel>
#include "resultfile.h"

ResultsProcessUI::ResultsProcessUI(QWidget *parent) :
	QWidget(parent, Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint),
	ui(new Ui::ResultsProcessUI)
{
	ui->setupUi(this);
	connect(ui->pushButton_export, SIGNAL(clicked()), this, SLOT(exportResultsPromp()));
	connect(ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(resetFiles()));
	connect(ui->pushButton_load, SIGNAL(clicked()), this, SLOT(loadFilesPromp()));
}

ResultsProcessUI::~ResultsProcessUI()
{
	delete ui;
}

void ResultsProcessUI::resetFiles(){
	for(ResultFile* result: results)
		delete result;
	results.clear();
	QLayoutItem* item = 0;
	while((item = ui->widget_archivos->layout()->takeAt(0))){
		delete item->widget();
		delete item;
	}
}

void ResultsProcessUI::loadFilesPromp(){
	QStringList filenames = QFileDialog::getOpenFileNames(this,
													("Open File"), "",
													"Results (*.s01 *.sl1)");
	if(filenames.size() == 0)
		return; // cancel
	for(QString filename: filenames){
		ui->widget_archivos->layout()->addWidget(new QLabel(filename));
		results.push_back(new ResultFile(filename.toStdString()));
	}
}

void ResultsProcessUI::exportResultsPromp(){
	QString dir = QFileDialog::getSaveFileName(this, "Save file", "",
											   "Text files (*.txt);;All files (*.*)");
	if(dir.size() == 0)
		return; // cancel
}
