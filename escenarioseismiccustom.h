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
		std::string seismic;
		std::string seismicv;
		bool enabled;
};

#endif // ESCENARIOCUSTOM_H
