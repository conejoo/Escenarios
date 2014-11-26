#include "escenarioseismiccustom.h"

EscenarioSeismicCustom::EscenarioSeismicCustom(int index, std::string n, std::string a):
	name(n),
	abbr(a)
{
	seismic = 0.0;
	seismicv = 0.0;
	this->index = index;
	enabled = true;
}
