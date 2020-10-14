#include "pch.h"
#include "Connection.h"
#include "utility/UI.h"

void Connection::UpdateInfo(std::string start, std::string end)
{
	this->start = start;
	this->end = end;
}

Log Connection::ChooseLog(size_t ID)
{
	Log log = allConnections.GetLog(ID);
	if (log.ID == -1)
		WrongChoice("No log with this ID found! ");
	return log;
}

std::vector<Log> Connection::ChooseBlock(size_t ID)
{
	std::vector<Log> block = startEndConnections.GetBlock(ID);
	if (block.size() == 0)
		WrongChoice("No block with this number found! ");
	return block;
}

void Connection::ShowAllCities()
{
	std::cout << citiesIDs << std::endl;
	std::cout << Clr(8) << "  " << citiesIDs.cities.size() << " cities found.\n" << Clr();
}

void Connection::ShowAllConnections()
{
	std::cout << allConnections << std::endl;
	std::cout << Clr(8) << "  " << allConnections.logNum << " connections found.\n" << Clr();
}

void Connection::ShowStartEndConnections()
{
	if (start == "" || end == "")
		WrongChoice("One of the start and end points is empty! ");
	else {
		std::cout << startEndConnections << std::endl;
		std::cout << Clr(8) << "  " << startEndConnections.logNum << " routes found.\n" << Clr();
	}
}

void RouteNodesToLogs(const std::vector<std::vector<RouteNode>>& routeNodes, const Database& srcDatabase, CitiesIDs& citiesIDs, Database& destDatabase)
{
	destDatabase.header = srcDatabase.header;
	for (const auto& col : routeNodes) {
		destDatabase.info.push_back(std::vector<Log>());
		for (const auto& rN : col) {
			Log foundLog = srcDatabase.GetLog(rN.routeID);
			foundLog.cityA = citiesIDs.dataI2S[rN.cityA_ID];
			foundLog.cityB = citiesIDs.dataI2S[rN.cityB_ID];
			destDatabase.info[destDatabase.info.size() - 1].push_back(foundLog);
		}
	}
}
