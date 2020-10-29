#include "pch.h"
#include "Log.h"

std::array<size_t, 7> columnWidths = { 5, 8, 5, 3, 9, 8, 5 };

Log::Log(const std::string& line)
{
	size_t i = 0;
	std::stringstream ss(line.c_str());
	std::vector<std::string> cs;
	std::string c;
	while (std::getline(ss, c, ';')) {
		cs.push_back(c);
		if (i != 4 && c.size() > columnWidths[i])
			columnWidths[i] = c.size() + 1;
		else if (i == 4) {
			size_t n = std::to_string((int)std::stof(c)).size() + 4;
			if (n > columnWidths[i])
				columnWidths[i] = n + 1;
		}
		i++;
	}
	ID = std::stoull(cs[0]);
	carrier = cs[1];
	cityA = cs[2];
	cityB = cs[3];
	duration = std::stof(cs[4]);
	cost = std::stof(cs[5]);
	type = cs[6].front();
}

void Log::UpdataIDsOfCities(const std::map<std::string, size_t>& S2I) {
	cityA_ID = S2I.at(cityA);
	cityB_ID = S2I.at(cityB);
}

bool Log::FilterPass() const {
	bool match = true;
	if (match && UI::filter.carriersActive && UI::filter.carriers.find(carrier) == std::string::npos)
		match = false;
	if (match && UI::filter.citiesActive && (UI::filter.cities.find(cityA) == std::string::npos && UI::filter.cities.find(cityB) == std::string::npos))
		match = false;
	if (match && UI::filter.durationActive && (UI::filter.duration.second < duration || duration < UI::filter.duration.first))
		match = false;
	if (match && UI::filter.costActive && (UI::filter.cost.second < cost || cost < UI::filter.cost.first))
		match = false;
	if (match && UI::filter.typesActive && UI::filter.types.find(type) == std::string::npos)
		match = false;
	return match;
}

std::ostream& operator<<(std::ostream& os, const Log& log) {
	if (!strcmp(typeid(os).name(), ostreamTypeName)) {
		os << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[0]) << Clr(log.color) << log.ID << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[1]) << Clr(log.color) << log.carrier << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[2]) << Clr(log.color) << log.cityA << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[3]) << Clr(log.color) << log.cityB << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[4]) << Clr(log.color) << Time(log.duration) << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[5]) << Clr(log.color) << Money(log.cost) << " " << Clr(BClr) << (char)-70 << Clr();
		os << std::setw(columnWidths[6]) << Clr(log.color) << log.type << " " << Clr(BClr) << (char)-70 << Clr();
	}
	else if (!strcmp(typeid(os).name(), ofstreamTypeName)) {
		os << log.ID << ';';
		os << log.carrier << ';';
		os << log.cityA << ';';
		os << log.cityB << ';';
		os << log.duration << ';';
		os << log.cost << ';';
		os << log.type << ';';
	}
	return os;
}
