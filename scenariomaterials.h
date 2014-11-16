#ifndef SCENARIOMATERIALS_H
#define SCENARIOMATERIALS_H

#include <QWidget>

namespace Ui {
class ScenarioMaterials;
}

class ScenarioMaterials : public QWidget
{
		Q_OBJECT

	public:
		explicit ScenarioMaterials(QWidget *parent = 0);
		~ScenarioMaterials();

	private:
		Ui::ScenarioMaterials *ui;
};

#endif // SCENARIOMATERIALS_H
