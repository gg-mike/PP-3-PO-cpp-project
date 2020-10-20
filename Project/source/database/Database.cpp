#include "pch.h"
#include "Database.h"

std::array<size_t, 6> columnWidths = { 3, 4, 3, 8, 7, 3 };

// ////////////////////
// CitiesIDs
// ////////////////////

void CitiesIDs::Init(const std::vector<std::vector<Log>>& info)
{
	size_t i = 0;
	for (const auto& block : info)
		for (const auto& log : block) {
			if (std::find(cities.begin(), cities.end(), log.cityA) == cities.end()) {
				cities.push_back(log.cityA);
				S2I[log.cityA] = i;
				I2S[i] = log.cityA;
				i++;
			}
			if (std::find(cities.begin(), cities.end(), log.cityB) == cities.end()) {
				cities.push_back(log.cityB);
				S2I[log.cityB] = i;
				I2S[i] = log.cityB;
				i++;
			}
		}
}

std::ostream& operator<<(std::ostream& os, const CitiesIDs& citiesIDs)
{
	os << Border(3, 10);
	for (const auto& c : citiesIDs.cities)
		os << "  " << c << std::endl;
	os << Border(5, 10);
	return os;
}

// ////////////////////
// Database
// ////////////////////

void Database::Init(const std::string& filepath, char type) {
	this->type = type;
	columnWidths = { 3, 4, 3, 8, 7, 3 };
	if (std::filesystem::exists(filepath)) {
		std::ifstream ifs(filepath);
		std::string line;
		bool isHeader = true;
		size_t i = 0;
		while (std::getline(ifs, line)) {
			std::stringstream ss(line.c_str());
			std::vector<std::string> cs;
			std::string c;
			if (isHeader) {
				while (std::getline(ss, c, ';')) {
					header.push_back(c);
					if (c.size() > columnWidths[i])
						columnWidths[i] = c.size();
					i++;
				}
				isHeader = false;
				info.push_back(std::vector<Log>());
			}
			else {
				if (std::toupper(line[0]) == 'B') {
					info.push_back(std::vector<Log>());
					MakeSummary();
				}
				else if (std::toupper(line[0]) == 'X')
					MakeSummary();
				else if ((type != 'M' && line.back() == type) || type == 'M') {
					logCount++;
					while (std::getline(ss, c, ';')) {
						cs.push_back(c);
						if (i != 4 && c.size() > columnWidths[i])
							columnWidths[i] = c.size();
						else if (i == 4) {
							size_t n = std::to_string((int)std::stof(c)).size() + 4;
							if (n > columnWidths[i])
								columnWidths[i] = n;
						}
						i++;
					}
					info.back().push_back(Log(std::stoull(cs[0]), cs[1], cs[2], std::stof(cs[3]), std::stof(cs[4]), cs[5][0]));
				}
			}
			i = 0;
		}
		for (auto& v : columnWidths)
			v++;
		ifs.close();

		citiesIDs.Init(info);
		for (auto& block : info)
			for (auto& log : block)
				log.UpdataIDsOfCities(citiesIDs.S2I);
		graph.Init(info, citiesIDs.cities.size());

	}
	else {
		std::cout << "Connection database is missing!\n"
			<< "Please reload the application with the new database provided to this location: \"" << filepath << "\"\n"
			<< "Or make sure that given filepath is correct.\n";
		std::cin.get();
		exit(1);
	}
}

void Database::Init(std::vector<std::vector<Log>>& info, char type)
{
	this->info = info;
	this->type = type;
	citiesIDs.Init(info);
	for (auto& block : info)
		for (auto& log : block)
			log.UpdataIDsOfCities(citiesIDs.S2I);
}

Log Database::GetLog(size_t ID) const {
	for (const auto& block : info)
		for (const auto& log : block)
			if (log.ID == ID)
				return log;
	return Log();
}

std::vector<Log> Database::GetBlock(size_t ID) const {
	std::vector<Log> block;
	for (size_t i = 0; i < info.size(); i++) {
		if (i == ID)
			return info[i];
	}
	return std::vector<Log>();
}

std::vector<Log> Database::GetLogs(const std::string& cityA, const std::string& cityB, char type) const {
	std::vector<Log> logs = {};
	for (const auto& block : info) {
		for (const auto& log : block)
			if ((log.type == type && type != 'M') || type == 'M') {
				if ((log.cityA == cityA || log.cityA == cityB) && (log.cityB == cityB || log.cityB == cityA))
					logs.push_back(log);
			}
	}
	return logs;
}

std::vector<Log> Database::GetLogs(size_t cityA_ID, size_t cityB_ID, char type) const {
	std::vector<Log> logs = {};
	for (const auto& block : info) {
		for (const auto& log : block) 
			if ((log.type == type && type != 'M') || type == 'M') {
				if ((log.cityA_ID == cityA_ID || log.cityA_ID == cityB_ID) && (log.cityB_ID == cityB_ID || log.cityB_ID == cityA_ID))
					logs.push_back(log);
			}
	}
	return logs;
}

bool Database::MakeSummary(size_t ID)
{
	if (ID == SIZE_MAX)
		ID = summaries.size();
	if (ID > summaries.size())
		return false;
	Log summary(ID + 1ull, info[ID][0].cityA, info[ID].back().cityB, 0, 0, info[ID][0].type, COLOR::DARK_YELLOW);
	for (const auto& log : info[ID]) {
		summary.duration += log.duration;
		summary.cost += log.cost;
		if (summary.type != log.type)
			summary.type = 'M';
	}
	if (ID == summaries.size())
		summaries.push_back(summary);
	else
		summaries[ID] = summary;
	return true;
}

void Database::AddBlock(std::vector<Log>& block)
{
	info.push_back(block);
}

void Database::EraseBlock(size_t ID)
{
	info.erase(info.begin() + ID);
}

std::ostream& operator<<(std::ostream& os, const Database& database) {
	os << Border(0);
	if (!database.info.size())
		os << Border(2);
	else {
		os << Clr(BClr) << (char)-70;
		// HEADER
		for (size_t i = 0; i < 6; i++)
			os << std::setw(columnWidths[i]) << Clr(COLOR::YELLOW) << database.header[i] << " " << Clr(BClr) << (char)-70;
		os << std::endl;
		if (database.info.size() == 1) {
			// LOGS
			for (const auto& log : database.info[0])
				os << Border(1) << log << std::endl;
			os << Border(2);
		}
		else {
			os << Border(2);
			for (size_t i = 0; i < database.info.size(); i++) {
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
