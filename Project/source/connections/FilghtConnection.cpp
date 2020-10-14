#include "pch.h"
#include "FlightConnection.h"

FlightConnection::FlightConnection()
{
	FindAllConnections();
}

void FlightConnection::UpdateInfo(std::string start, std::string end)
{
	Connection::UpdateInfo(start, end);
	FindStartEndConnections();
}

void FlightConnection::FindAllConnections()
{
	allConnections.FromFile("assets/databases/connections.csv", 'F');
	citiesIDs.FromDatabase(allConnections);
	allConnections.UpdataIDsInInfo(citiesIDs.dataS2I);
}

void FlightConnection::FindStartEndConnections()
{
	connectionsGraph.FromDatabase(allConnections, citiesIDs.cities.size(), 'F');
	std::vector<std::vector<RouteNode>> results;
	connectionsGraph.FindConnections(start, end, results);
	if (results.size())
		RouteNodesToLogs(results, allConnections, citiesIDs, startEndConnections);
}
