#ifndef ESCENARIOCUSTOM_H
#define ESCENARIOCUSTOM_H

#include <string>

class EscenarioSeismicCustom
{
	public:
		EscenarioSeismicCustom(int index, std::wstring n, std::string a);
		int index;
		std::wstring name;
		std::string abbr;
		double seismic;
		double seismicv;
		bool enabled;
};

#endif // ESCENARIOCUSTOM_H
