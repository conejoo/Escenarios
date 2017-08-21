#include "customfiledialognewlayout.h"
#include "ui_customfiledialognewlayout.h"

#include <QPushButton>
#include <iostream>
#include <QListWidgetItem>
#include <QStringList>

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
	file_dialog.setLabelText(QFileDialog::LookIn, "Buscar en");
	file_dialog.setLabelText(QFileDialog::FileName, "Archivo");
	file_dialog.setLabelText(QFileDialog::FileType, "Tipo de archivo");
	QStringList list = (QStringList() << "Resultados (*.s01)" << "Todos los archivos (*)");
	file_dialog.setNameFilters(list);
	//file_dialog.open(this, SLOT(openProcessFiles(const QStringList &)));
	connect(&file_dialog, SIGNAL(filesSelected(QStringList)), this, SLOT(addFilesToList(const QStringList &)));
	connect(ui->pushButton_add_one, SIGNAL(clicked()), this, SLOT(addFilesToListArrow()));
	connect(ui->pushButton_add_all, SIGNAL(clicked()), this, SLOT(addAllFilesToListArrow()));
	connect(ui->pushButton_quitar, SIGNAL(clicked()), this, SLOT(removeFilenames()));
	connect(ui->pushButton_remove_one, SIGNAL(clicked()), this, SLOT(removeFilenames()));
	connect(ui->pushButton_remove_all, SIGNAL(clicked()), this, SLOT(removeAllFilenames()));
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

void CustomFileDialogNewLayout::addFilesToListArrow(){
	addFilesToList(file_dialog.selectedFiles());
}

void CustomFileDialogNewLayout::addAllFilesToListArrow(){
	QStringList name_filters;
	if(file_dialog.selectedNameFilter().indexOf(QString(".s01")) != -1){
		name_filters << "*.s01";
	}
	QStringList absolute_file_paths;
	for(QString filename: file_dialog.directory().entryList(name_filters, QDir::Files)){
		absolute_file_paths << file_dialog.directory().absoluteFilePath(filename);
	}
	addFilesToList(absolute_file_paths);
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

void CustomFileDialogNewLayout::removeAllFilenames(){
	ui->tableWidget->selectAll();
	removeFilenames();
}
