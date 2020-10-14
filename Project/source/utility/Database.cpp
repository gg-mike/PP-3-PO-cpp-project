#include "pch.h"
#include "Database.h"
#include "UI.h"

std::array<size_t, 6> logWidths = { 3, 4, 3, 8, 7, 3 };

Log Database::GetLog(const std::string& cityA, const std::string& cityB, char type) const {
	for (const auto& block : info) {
		for (const auto& log : block)
			if (log.type == type) {
				if ((log.cityA == cityA || log.cityA == cityB) && (log.cityB == cityB || log.cityB == cityA))
					return log;
			}
	}
	return Log();
}

Log Database::GetLog(int cityA_ID, int cityB_ID, char type) const {
	for (const auto& block : info) {
		for (const auto& log : block) 
			if (log.type == type) {
				if ((log.cityA_ID == cityA_ID || log.cityA_ID == cityB_ID) && (log.cityB_ID == cityB_ID || log.cityB_ID == cityA_ID))
					return log;
			}
	}
	return Log();
}

Log Database::GetLog(size_t ID) const {
	for (const auto& block : info)
		for (const auto& log : block)
			if (log.ID == ID)
				return log;
	return Log();
}

std::vector<Log> Database::GetBlock(size_t ID) {
	std::vector<Log> block;
	for (size_t i = 0; i < info.size(); i++) {
		if (i == ID)
			return info[i];
	}
	return std::vector<Log>();
}

void Database::MakeSummaryLog(size_t blockID)
{
	if (blockID == INT_MAX)
		blockID = summaries.size();
	Log summary(static_cast<int>(blockID), info[blockID][0].cityA, info[blockID][info[blockID].size() - 1].cityB, 0, 0, info[blockID][0].type, 6);
	for (const auto& log : info[blockID]) {
		summary.duration += log.duration;
		summary.cost += log.cost;
		if (summary.type != log.type)
			summary.type = 'M';
	}
	if (blockID >= summaries.size())
		summaries.push_back(summary);
	else
		summaries[blockID] = summary;
}

void Database::FromFile(const std::string& filepath, char type) {
	logWidths = { 3, 4, 3, 8, 7, 3 };
	if (std::filesystem::exists(filepath)) {
		std::ifstream ifs(filepath);
		std::string line;
		bool isHeader = true;
		int i = 0;
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
				info.push_back(std::vector<Log>());
			}
			else {
				if (std::toupper(line[0]) == 'B') {
					info.push_back(std::vector<Log>());
					MakeSummaryLog();
				}
				else if (std::toupper(line[0]) == 'X')
					MakeSummaryLog();
				else if ((type != 'M' && line[line.size() - 1] == type) || type == 'M') {
					logNum++;
					while (std::getline(ss, c, ';')) {
						cs.push_back(c);
						if (i != 4 && c.size() > logWidths[i])
							logWidths[i] = c.size();
						else if (i == 4) {
							int n = std::to_string((int)std::stof(c)).size() + 4;
							if (n > logWidths[i])
								logWidths[i] = n;
						}
						i++;
					}
					size_t maxC = info.size() - 1;
					info[maxC].push_back(Log(std::stoi(cs[0]), cs[1], cs[2], std::stof(cs[3]), std::stof(cs[4]), cs[5][0]));
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
	for (auto& block : info)
		for (auto& log : block)
			log.UpdataIDs(citiesS2I);
}

std::ostream& operator<<(std::ostream& os, const Database& database) {
	os << Border(0);
	if (!database.info.size())
		os << Border(2);
	else {
		os << Clr(BClr) << (char)-70;
		// HEADER
		for (int i = 0; i < 6; i++)
			os << std::setw(logWidths[i]) << Clr(14) << database.header[i] << " " << Clr(BClr) << (char)-70;
		os << std::endl;
		if (database.info.size() == 1) {
			// LOGS
			for (const auto& log : database.info[0])
				os << Border(1) << log << std::endl;
			os << Border(2);
		}
		else {
			os << Border(2);
			for (int i = 0; i < database.info.size(); i++) {
				// SUMMARY
				os << Border(0) << database.summaries[i] << std::endl;
				// LOGS
				for (const auto& log : database.info[i])
					os << Border(1) << log << std::endl;
				os << Border(2);
			}
		}
	}
	return os;
}

void CitiesIDs::FromDatabase(const Database& database)
{
	int i = 0;
	for (const auto& block : database.info)
		for (const auto& log : block) {
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
	os << Border(3, 10);
	for (const auto& c : citiesIDs.cities)
		os << "  " << c << std::endl;
	os << Border(5, 10);
	return os;
}
