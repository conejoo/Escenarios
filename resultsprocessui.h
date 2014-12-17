#ifndef RESULTSPROCESSUI_H
#define RESULTSPROCESSUI_H

#include <QWidget>
#include <vector>
#include <unordered_map>
#include "escenariofile.h"
#include "customfiledialognewlayout.h"

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
		void openProcessFiles(const QStringList & filenames);

	private:
		void countAddedFiles();
		std::wstring translateMaterialScenario(std::string abbr);
		bool isMedMaterialScenario(std::string abbr);
		bool areTranslatables(std::string abbr_se, std::string abbr_mat);
		std::wstring translateSeismicScenario(std::string abbr);
		std::wstring localTranslate(std::string abbr);
		std::wstring toWString(std::string);
		Ui::ResultsProcessUI *ui;
		std::vector<ResultFile*> results;
		std::unordered_map<std::string, ResultFile*> results_map;
		std::string addUnit(const char* name);
		std::unordered_map<std::string, std::wstring> translation_scenario;
		EscenarioFile& escenarios;
		CustomFileDialogNewLayout custom_file_dialog;
};

#endif // RESULTSPROCESSUI_H
