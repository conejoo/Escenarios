#ifndef CUSTOMFILEDIALOGNEWLAYOUT_H
#define CUSTOMFILEDIALOGNEWLAYOUT_H

#include <QWidget>
#include <QLayoutItem>
#include <QFileDialog>
#include <QSet>

namespace Ui {
class CustomFileDialogNewLayout;
}

class CustomFileDialogNewLayout : public QWidget
{
		Q_OBJECT

	public:
		explicit CustomFileDialogNewLayout(QWidget *parent = 0);
		~CustomFileDialogNewLayout();

	public slots:
		void addFilesToList(const QStringList & selected);
		void removeFilenames();
		void openDialog();
		void processNewFiles();

	signals:
		void newFileSelection(const QStringList & selected);

	private:
		Ui::CustomFileDialogNewLayout *ui;
		int n;
		QFileDialog file_dialog;
		QSet<QString> filenames;
};

#endif // CUSTOMFILEDIALOGNEWLAYOUT_H
