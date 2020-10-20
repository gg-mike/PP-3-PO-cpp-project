#include "pch.h"
#include "Connection.h"
#include "utility/UI.h"

void Connection::Init(std::string start, std::string end, bool isWeighted, char type)
{
	this->start = start;
	this->end = end;
	FindConnections(isWeighted, type);
}

Log Connection::GetLog(size_t ID)
{
	Log log = database.GetLog(ID);
	return log;
}

std::vector<Log> Connection::GetBlock(size_t ID)
{
	std::vector<Log> block = startEndConnections.GetBlock(ID);
	return block;
}

void Connection::ShowAllCities()
{
	std::cout << database.GetCitiesIDs() << std::endl;
	std::cout << Clr(COLOR::DARK_GREY) << "  " << database.GetCitiesIDs().cities.size() << " cities found.\n" << Clr();
}

void Connection::ShowDatabase()
{
	std::cout << database << std::endl;
	std::cout << Clr(COLOR::DARK_GREY) << "  " << database.GetLogCount() << " connections found.\n" << Clr();
}

void Connection::ShowStartEndConnections()
{
	if (start == "" || end == "")
		WrongChoice("One of the start and end points is empty! ");
	else {
		std::cout << startEndConnections << std::endl;
		std::cout << Clr(COLOR::DARK_GREY) << "  " << startEndConnections.GetLogCount() << " routes found.\n" << Clr();
	}
}

bool Connection::IsValidCity(std::string city)
{
	if (std::find(database.GetCitiesIDs().cities.begin(), database.GetCitiesIDs().cities.end(), city) != database.GetCitiesIDs().cities.end())
		return true;
	return false;
}

void Connection::PathToDatabase(const std::vector<size_t>& path, bool isWeighted, char type)
{
	// TODO: Generate connections
}

void Connection::FindConnections(bool isWeighted, char type)
{
	std::vector<size_t> path;
	size_t startID = database.GetCitiesIDs().S2I.at(start);
	size_t endID = database.GetCitiesIDs().S2I.at(end);

	if (isWeighted) {
		database.GetGraph().InitMST(database.GetInfo(), type);
		database.GetGraph().FindConnectionsWeighted(startID, endID, path);
	}
	else
		database.GetGraph().FindConnections(startID, endID, path);

	PathToDatabase(path, isWeighted, type);
}
