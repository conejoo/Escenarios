#ifndef ESCENARIOCUSTOM_H
#define ESCENARIOCUSTOM_H

#include <string>

class EscenarioSeismicCustom
{
	public:
		EscenarioSeismicCustom(int index, std::string n, std::string a);
		int index;
		std::string name;
		std::string abbr;
		double seismic;
		double seismicv;
		bool enabled;
};

#endif // ESCENARIOCUSTOM_H
