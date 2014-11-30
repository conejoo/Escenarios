#ifndef ESCENARIOMATERIALCUSTOM_H
#define ESCENARIOMATERIALCUSTOM_H

#include <string>

class EscenarioMaterialCustom
{
	public:
		EscenarioMaterialCustom(int index, std::wstring n, std::string a);
		int index;
		std::wstring name;
		std::string abbr;
		bool enabled;
};

#endif // ESCENARIOMATERIALCUSTOM_H
