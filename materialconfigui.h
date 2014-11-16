#ifndef MATERIALCONFIGUI_H
#define MATERIALCONFIGUI_H

#include <QWidget>
#include <QLineEdit>
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

	private:
		Ui::MaterialConfigUI *ui;
		Material& material;
		int material_index;
		bool read_only;
		std::vector<QLineEdit*> line_edits;
};

#endif // MATERIALCONFIGUI_H
