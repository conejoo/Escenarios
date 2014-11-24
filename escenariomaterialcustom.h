#ifndef ESCENARIOMATERIALCUSTOM_H
#define ESCENARIOMATERIALCUSTOM_H

#include <string>

class EscenarioMaterialCustom
{
	public:
		EscenarioMaterialCustom(int index, std::string n, std::string a);
		int index;
		std::string name;
		std::string abbr;
		bool enabled;
};

#endif // ESCENARIOMATERIALCUSTOM_H
