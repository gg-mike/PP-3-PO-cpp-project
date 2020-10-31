#include "pch.h"
#include "Connection.h"
#include "utility/UI.h"

// ////////////////////
// Utility
// ////////////////////

void PathOptions(size_t i, const std::vector<std::vector<Log>>& available, std::vector<Log> curr, std::vector<std::vector<Log>>& info) {
	if (i >= available.size())
		info.push_back(curr);
	else {
		for (auto& log : available[i]) {
			curr.push_back(log);
			PathOptions(i + 1ull, available, curr, info);
			curr.pop_back();
		}
	}
}

// ////////////////////
// Connection
// ////////////////////

void Connection::Init(std::string start, std::string end, char type) {
	this->start = start;
	this->end = end;
	FindConnections(type);
}

std::string Connection::GetCityName(size_t ID) {
	return database.GetCitiesIDs().cities.at(ID);
}

Log Connection::GetLogD(size_t ID) {
	Log log = database.GetLog(ID);
	return log;
}

std::vector<Log> Connection::GetBlockSE(size_t ID) {
	std::vector<Log> block = startEndConnections.GetBlock(ID);
	return block;
}

void Connection::ShowAllCities() {
	std::cout << database.GetCitiesIDs();
	std::cout << Clr(COLOR::DARK_GREY) << "  Number of cities found: " << database.GetCitiesIDs().cities.size() << Clr() << std::endl << std::endl;
}

void Connection::ShowDatabase() {
	std::cout << database << std::endl;
}

void Connection::ShowStartEndConnections() {
	if (start == "" || end == "")
		WrongChoice("One of the start and end points is empty! ");
	else
		std::cout << startEndConnections << std::endl;
}

bool Connection::IsValidCity(size_t cityID) {
	if (cityID >= database.GetCitiesIDs().cities.size())
		return false;
	return true;
}

bool Connection::IsValidRouteID(size_t ID) {
	if (ID >= startEndConnections.GetInfo().size())
		return false;
	return true;
}

void Connection::PathToDatabase(const std::vector<size_t>& path, char type) {
	std::vector<std::vector<Log>> available;
	std::vector<std::vector<Log>> info = {};

	if (path.size()) {
		for (size_t i = 0; i < path.size() - 1; i++)
			available.push_back(database.GetLogs(path[i], path[i + 1ull]));
		PathOptions(0, available, {}, info);
	}

	startEndConnections.Init(database.GetHeader(), info, 'M', true);
	startEndConnections.SortBlock(type);
}

void Connection::FindConnections(char type) {
	std::vector<size_t> path;
	size_t startID = database.GetCitiesIDs().S2I.at(start);
	size_t endID = database.GetCitiesIDs().S2I.at(end);

	database.GetGraph().FindConnections(startID, endID, type, path);

	PathToDatabase(path, type);
}
