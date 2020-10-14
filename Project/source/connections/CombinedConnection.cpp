#include "pch.h"
#include "CombinedConnection.h"

CombinedConnection::CombinedConnection()
{
	FindAllConnections();
}

void CombinedConnection::UpdateInfo(std::string start, std::string end)
{
	Connection::UpdateInfo(start, end);
	FindStartEndConnections();
}

void CombinedConnection::FindAllConnections()
{
	allConnections.FromFile("assets/databases/connections.csv");
	citiesIDs.FromDatabase(allConnections);
	allConnections.UpdataIDsInInfo(citiesIDs.dataS2I);
}

void CombinedConnection::FindStartEndConnections()
{
	connectionsGraph.FromDatabase(allConnections, citiesIDs.cities.size(), 'M');
	std::vector<std::vector<RouteNode>> results;
	connectionsGraph.FindConnections(start, end, results);
	if (results.size())
		RouteNodesToLogs(results, allConnections, citiesIDs, startEndConnections);
}
