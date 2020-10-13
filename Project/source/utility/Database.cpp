#include "pch.h"
#include "Database.h"
#include "UI.h"

std::array<int, 6> logWidths = { 3, 3, 3, 3, 3, 3 };

int Database::FindLog(const std::string& cityA, const std::string& cityB, char type)
{
	for (const auto& log : info) {
		if (log.type == type) {
			if ((log.cityA == cityA || log.cityA == cityB) && (log.cityB == cityB || log.cityB == cityA))
				return log.ID;
		}
	}
	return -1;
}

int Database::FindLog(int cityA_ID, int cityB_ID, char type)
{
	for (const auto& log : info) {
		if (log.type == type) {
			if ((log.cityA_ID == cityA_ID || log.cityA_ID == cityB_ID) && (log.cityB_ID == cityB_ID || log.cityB_ID == cityA_ID))
				return log.ID;
		}
	}
	return -1;
}

void Database::FromFile(const std::string& filepath, char type) {

	if (std::filesystem::exists(filepath)) {
		std::ifstream ifs(filepath);
		std::string line;
		bool isHeader = true;
		int i = 0;
		int count = 0;
		while (std::getline(ifs, line)) {
			std::stringstream ss(line.c_str());
			std::vector<std::string> cs;
			std::string c;
			if (isHeader) {
				while (std::getline(ss, c, ';')) {
					header.push_back(c);
					if (c.size() > logWidths[i])
						logWidths[i] = c.size();
					i++;
				}
				isHeader = false;
			}
			else {
				if (std::toupper(line[0]) == 'X') {
					splits.push_back(count);
					count = 0;
				}
				else if ((type != 'M' && line[line.size() - 1] == type) || type == 'M') {
					while (std::getline(ss, c, ';')) {
						cs.push_back(c);
						if (c.size() > logWidths[i])
							logWidths[i] = c.size();
						i++;
					}
					count++;
					info.push_back(Log(std::stoi(cs[0]), cs[1], cs[2], std::stof(cs[3]), std::stof(cs[4]), cs[5][0]));
				}
			}
			i = 0;
		}
		for (auto& v : logWidths)
			v++;
		ifs.close();
	}
	else {
		std::cout << "Connection database is missing!\n"
			<< "Please reload the application with the new database provided to this location: \"" << filepath << "\"\n"
			<< "Or make sure that given filepath is correct.\n";
		std::cin.get();
		exit(1);
	}
}

void Database::UpdataIDsInInfo(std::map<std::string, int>& citiesS2I)
{
	for (auto& l : info)
		l.UpdataIDs(citiesS2I);
}

std::ostream& operator<<(std::ostream& os, const Database& database) {
	os << Border(0) << Clr(BClr) << (char)-70;
	for (int i = 0; i < 6; i++)
		os << std::setw(logWidths[i]) << Clr(14) << database.header[i] << " " << Clr(BClr) << (char)-70;
	os << std::endl;
	int i = 0;
	int lNum = 0;
	for (auto s : database.splits) {
		os << Border(2) << Clr(14) << lNum << Clr() << std::endl << Border(0);
		for (int c = 0; c < s; c++, i++)
			os << database.info[i] << std::endl;
		lNum++;
	}
	os << Border(2);
	return os;
}

void CitiesIDs::FromDatabase(const Database& database)
{
	int i = 0;
	for (const auto& log : database.info) {
		if (std::find(cities.begin(), cities.end(), log.cityA) == cities.end()) {
			cities.push_back(log.cityA);
			dataS2I[log.cityA] = i;
			dataI2S[i] = log.cityA;
			i++;
		}
		if (std::find(cities.begin(), cities.end(), log.cityB) == cities.end()) {
			cities.push_back(log.cityB);
			dataS2I[log.cityB] = i;
			dataI2S[i] = log.cityB;
			i++;
		}
	}
}

std::ostream& operator<<(std::ostream& os, CitiesIDs& citiesIDs)
{
	for (const auto& c : citiesIDs.cities)
		os << c << " : " << citiesIDs.dataS2I[c] << std::endl;
	return os;
}
