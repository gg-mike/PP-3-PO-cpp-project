#include "pch.h"
#include "CruiseConnection.h"

CruiseConnection::CruiseConnection()
{
	FindAllConnections();
}

void CruiseConnection::UpdateInfo(std::string start, std::string end)
{
	Connection::UpdateInfo(start, end);
	FindStartEndConnections();
}

void CruiseConnection::FindAllConnections()
{
	allConnections.FromFile("assets/databases/connections.csv", 'C');
	citiesIDs.FromDatabase(allConnections);
	allConnections.UpdataIDsInInfo(citiesIDs.dataS2I);
}

void CruiseConnection::FindStartEndConnections()
{
	connectionsGraph.FromDatabase(allConnections, citiesIDs.cities.size(), 'C');
	std::vector<std::vector<RouteNode>> results;
	connectionsGraph.FindConnections(start, end, results);
	if (results.size())
		RouteNodesToLogs(results, allConnections, citiesIDs, startEndConnections);
}
