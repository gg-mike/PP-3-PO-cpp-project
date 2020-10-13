#include "pch.h"
#include "Log.h"
#include "UI.h"

void Log::UpdataIDs(std::map<std::string, int>& citiesS2I)
{
	cityA_ID = citiesS2I[cityA];
	cityB_ID = citiesS2I[cityB];
}

std::ostream& operator<<(std::ostream& os, const Log& log)
{
	if (!strcmp(typeid(os).name(), ostreamTypeName)) {
		os << Border(1) << Clr(BClr) << (char)-70;
		os << std::setw(logWidths[0]) << log.ID << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(logWidths[1]) << log.cityA << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(logWidths[2]) << log.cityB << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(logWidths[3]) << log.duration << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(logWidths[4]) << log.cost << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(logWidths[5]) << log.type << " " << Clr(BClr) << (char)-70 << Clr();
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