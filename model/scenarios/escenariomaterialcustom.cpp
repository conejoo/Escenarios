#include "escenariomaterialcustom.h"

EscenarioMaterialCustom::EscenarioMaterialCustom(int index, std::wstring n, std::string a):
	name(n),
	abbr(a)
{
	this->index = index;
	enabled = true;
}
