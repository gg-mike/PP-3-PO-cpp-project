#include "pch.h"
#include "Log.h"

void Log::UpdataIDsOfCities(const std::map<std::string, size_t>& S2I)
{
	cityA_ID = S2I.at(cityA);
	cityB_ID = S2I.at(cityB);
}

std::ostream& operator<<(std::ostream& os, const Log& log)
{
	if (!strcmp(typeid(os).name(), ostreamTypeName)) {
		os << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[0]) << Clr(log.color) << log.ID << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[1]) << Clr(log.color) << log.cityA << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[2]) << Clr(log.color) << log.cityB << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[3]) << Clr(log.color) << Time(log.duration) << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[4]) << Clr(log.color) << Money(log.cost) << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[5]) << Clr(log.color) << log.type << " " << Clr(BClr) << (char)-70 << Clr();
	}
	else if (!strcmp(typeid(os).name(), ofstreamTypeName)) {
		os << log.ID << ';';
		os << log.cityA << ';';
		os << log.cityB << ';';
		os << log.duration << ';';
		os << log.cost << ';';
		os << log.type << ';';
	}
	return os;
}
