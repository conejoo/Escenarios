#include "customfiledialognewlayout.h"
#include "ui_customfiledialognewlayout.h"

#include <QPushButton>
#include <iostream>
#include <QListWidgetItem>

CustomFileDialogNewLayout::CustomFileDialogNewLayout(QWidget *parent) :
	QWidget(parent, Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint),
	ui(new Ui::CustomFileDialogNewLayout),
	file_dialog()
{
	ui->setupUi(this);
	n = 0;
	file_dialog.setOption(QFileDialog::DontUseNativeDialog);
	file_dialog.setFileMode(QFileDialog::ExistingFiles);
	file_dialog.setLabelText(QFileDialog::Accept, "Agregar");
	file_dialog.setLabelText(QFileDialog::Reject, "Cancelar");
	//file_dialog.open(this, SLOT(openProcessFiles(const QStringList &)));
	connect(&file_dialog, SIGNAL(filesSelected(QStringList)), this, SLOT(addFilesToList(const QStringList &)));
	connect(ui->pushButton_quitar, SIGNAL(clicked()), this, SLOT(removeFilenames()));
	connect(ui->pushButton_aceptar, SIGNAL(clicked()), this, SLOT(processNewFiles()));
	ui->widget_dialog->setLayout(file_dialog.layout());
}

CustomFileDialogNewLayout::~CustomFileDialogNewLayout()
{
	delete ui;
}

void CustomFileDialogNewLayout::processNewFiles(){
	this->close();
	emit newFileSelection(filenames.toList());
}

void CustomFileDialogNewLayout::addFilesToList(const QStringList & selected){
	for(QString filename: selected){
		if(filenames.contains(filename))
			continue;
		filenames.insert(filename);
		ui->tableWidget->addItem(filename);
	}
}

void CustomFileDialogNewLayout::openDialog(){
	filenames.clear();
	QListWidgetItem* item;
	while((item = ui->tableWidget->takeItem(0)))
		delete item;
	this->show();
}

void CustomFileDialogNewLayout::removeFilenames(){
	QList<QListWidgetItem*> selected_list = ui->tableWidget->selectedItems();
	for(QListWidgetItem* item: selected_list){
		QString filename = item->text();
		filenames.remove(filename);
		ui->tableWidget->removeItemWidget(item);
		delete item;
	}
}
