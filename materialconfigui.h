#ifndef MATERIALCONFIGUI_H
#define MATERIALCONFIGUI_H

#include <QWidget>
#include <QLineEdit>
#include <unordered_map>
#include "material.h"

namespace Ui {
class MaterialConfigUI;
}

class MaterialConfigUI : public QWidget
{
		Q_OBJECT

	public:
		explicit MaterialConfigUI(QWidget *parent, Material& material, int material_index, bool read_only);
		~MaterialConfigUI();

	public slots:
		void updatePropertyValue(QString new_value);

	private:
		Ui::MaterialConfigUI *ui;
		Material& material;
		int material_index;
		bool read_only;
		std::unordered_map<QLineEdit*, MaterialProperty*> line_edits_material;
};

#endif // MATERIALCONFIGUI_H
