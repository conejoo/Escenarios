#ifndef RESULTSPROCESSUI_H
#define RESULTSPROCESSUI_H

#include <QWidget>
#include <vector>

class ResultFile;

namespace Ui {
class ResultsProcessUI;
}

class ResultsProcessUI : public QWidget
{
		Q_OBJECT

	public:
		explicit ResultsProcessUI(QWidget *parent = 0);
		~ResultsProcessUI();

	public slots:
		void resetFiles();
		void loadFilesPromp();
		void exportResultsPromp();

	private:
		Ui::ResultsProcessUI *ui;
		std::vector<ResultFile*> results;
};

#endif // RESULTSPROCESSUI_H
