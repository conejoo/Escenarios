#ifndef RESULTSPROCESSUI_H
#define RESULTSPROCESSUI_H

#include <QWidget>
#include <vector>
#include <unordered_map>
#include "escenariofile.h"

class ResultFile;

namespace Ui {
class ResultsProcessUI;
}

class ResultsProcessUI : public QWidget
{
		Q_OBJECT

	public:
		explicit ResultsProcessUI(EscenarioFile& es, QWidget *parent = 0);
		~ResultsProcessUI();
		void setEscenarioFile(EscenarioFile&);

	public slots:
		void resetFiles();
		void loadFilesPromp();
		void exportResultsPromp();
		void removeResultFile();
		void addResultFile();
		void addAllResultFiles();
		void removeAllResultFiles();

	private:
		void countAddedFiles();
		std::wstring translateMaterialScenario(std::string abbr);
		std::wstring translateSeismicScenario(std::string abbr);
		std::wstring localTranslate(std::string abbr);
		std::wstring toWString(std::string);
		Ui::ResultsProcessUI *ui;
		std::vector<ResultFile*> results;
		std::unordered_map<std::string, ResultFile*> results_map;
		std::string addUnit(const char* name);
		std::unordered_map<std::string, std::wstring> translation_scenario;
		EscenarioFile& escenarios;
};

#endif // RESULTSPROCESSUI_H
