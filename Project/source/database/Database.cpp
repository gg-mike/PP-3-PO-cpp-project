#include "pch.h"
#include "Database.h"

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
	std::sort(cities.begin(), cities.end());
}

std::ostream& operator<<(std::ostream& os, const CitiesIDs& citiesIDs)
{
	os << Border(0, 5ull +((columnWidths[2] > columnWidths[3])? columnWidths[2] : columnWidths[3]));
	size_t i = 1;
	for (const auto& c : citiesIDs.cities)
		os << "  " << std::setw(2) << i++ << ". " << c << " " << std::endl;
	os << Border(2, 5ull + ((columnWidths[2] > columnWidths[3]) ? columnWidths[2] : columnWidths[3]));
	return os;
}

// ////////////////////
// Database
// ////////////////////

void Database::Init(const std::string& filepath, char type, bool showSummaries) {
	this->type = type;
	this->showSummaries = showSummaries;
	if (std::filesystem::exists(filepath)) {
		std::ifstream ifs(filepath);
		std::string line;

		while (std::getline(ifs, line)) {
			// End of block
			if (std::toupper(line[0]) == 'B')
				info.push_back(std::vector<Log>());
			// Log
			else if ((type != 'M' && line.back() == type) || type == 'M') {
				logCount++;
				if (info.empty())
					info.push_back(std::vector<Log>());
				info.back().push_back(Log(line));
			}
		}

		ifs.close();

		for (size_t i = 0; i < info.size(); i++) {
			summaries.push_back(Log());
			MakeSummary(i);
		}

		citiesIDs.Init(info);
		for (auto& block : info)
			for (auto& log : block)
				log.UpdataIDsOfCities(citiesIDs.S2I);
		graph.Init(info, citiesIDs.cities.size());
	}
	else {
		std::cout << "Connection database is missing!\n"
			<< "Please reload the application with the new database provided to this location: " << Clr(COLOR::YELLOW) << filepath << Clr() << std::endl
			<< "Or make sure that given filepath is correct.\n";
		std::cin.get();
		exit(1);
	}
}

void Database::Init(const std::vector<std::string>& header, std::vector<std::vector<Log>>& info, char type, bool showSummaries)
{
	this->header = header;
	this->info = info;
	this->type = type;
	this->summaries.clear();

	if (info.size()) {
		this->showSummaries = showSummaries;

		for (size_t i = 0; i < info.size(); i++) {
			summaries.push_back(Log());
			MakeSummary(i);
		}

		citiesIDs.Init(info);
		for (auto& block : info)
			for (auto& log : block) {
				logCount++;
				log.UpdataIDsOfCities(citiesIDs.S2I);
			}
	}
	else
		this->showSummaries = false;
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
					logs.push_back(Log(log.ID, log.carrier, cityA, cityB, log.duration, log.cost, log.type));
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
					logs.push_back(Log(log.ID, log.carrier, citiesIDs.I2S.at(cityA_ID), citiesIDs.I2S.at(cityB_ID), log.duration, log.cost, log.type));
			}
	}
	return logs;
}

bool Database::MakeSummary(size_t ID) {
	if (ID == SIZE_MAX) {
		ID = info.size() - 1;
		summaries.push_back(Log());
	}
	if (ID >= summaries.size())
		return false;
	Log summary(ID + 1ull, info[ID].front().carrier, info[ID].front().cityA, info[ID].back().cityB, 0, 0, info[ID].front().type, COLOR::DARK_YELLOW);
	for (const auto& log : info[ID]) {
		summary.duration += log.duration;
		summary.cost += log.cost;
		if (summary.type != log.type)
			summary.type = 'M';
		if (summary.carrier != log.carrier)
			summary.carrier = "Mixed";
	}
	summaries[ID] = summary;
	return true;
}

void Database::AddBlock(std::vector<Log>& block) {
	info.push_back(block);
}

void Database::AppendLog(Log& log) {
	info.back().push_back(log);
}

void Database::EraseBlock(size_t ID) {
	if (ID == SIZE_MAX)
		info.erase(info.end() - 1ull);
	else
		info.erase(info.begin() + ID);
}

struct Block {
	Log summary;
	std::vector<Log> data;
};

bool compD(const Block& a, const Block& b) {
	if (a.summary.duration != b.summary.duration)
		return a.summary.duration < b.summary.duration;
	else
		return a.summary.cost < b.summary.cost;
}

bool compC(const Block& a, const Block& b) {
	if (a.summary.cost != b.summary.cost)
		return a.summary.cost < b.summary.cost;
	else
		return a.summary.duration < b.summary.duration;
}

void Database::SortBlock(char type) {
	std::vector<Block> blocks(info.size());

	for (size_t i = 0; i < info.size(); i++) {
		blocks[i].summary = summaries[i];
		blocks[i].data = info[i];
	}

	switch (type)
	{
	case 'D': std::sort(blocks.begin(), blocks.end(), &compD); break;
	case 'C': std::sort(blocks.begin(), blocks.end(), &compC); break;
	}

	for (size_t i = 0; i < info.size(); i++) {
		summaries[i] = blocks[i].summary;
		summaries[i].ID = i;
		info[i] = blocks[i].data;
	}
}

std::ostream& operator<<(std::ostream& os, const Database& database) {
	size_t logsPrinted = 0;

	if (!database.showSummaries)
		os << UI::filter << std::endl;
	os << Border(0);
	// HEADER
	if (database.header.size() == columnWidths.size()) {
		os << Clr(BClr) << (char)-70;
		for (size_t i = 0; i < columnWidths.size(); i++)
			os << std::setw(columnWidths[i]) << Clr(COLOR::YELLOW) << database.header[i] << " " << Clr(BClr) << (char)-70;
		os << std::endl;
	}
	if (database.info.size() == 0)
		os << Border(2);
	else {
		if (!database.showSummaries) {
			// LOGS
			for (const auto& block : database.info)
				for (const auto& log : block) {
					if (UI::filter.active) {
						if (log.FilterPass()) {
							os << Border(1) << log << std::endl;
							logsPrinted++;
						}
					}
					else {
						os << Border(1) << log << std::endl;
						logsPrinted++;
					}
				}
			os << Border(2);
		}
		else {
			os << Border(2);
			size_t i = 0;
			for (const auto& block : database.info) {
				// SUMMARY
				os << Border(0);
				os << database.summaries[i] << std::endl;
				// LOGS
				for (const auto& log : block)
					os << Border(1) << log << std::endl;
				os << Border(2);
				i++;
			}
		}
	}
	if (!database.showSummaries)
		os << Clr(COLOR::DARK_GREY) << "  Number of connections found: " << logsPrinted << Clr() << std::endl;
	else
		os << Clr(COLOR::DARK_GREY) << "  Number of reservations: " << database.info.size() << Clr() << std::endl;
	return os;
}
