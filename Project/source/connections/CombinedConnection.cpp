#include "pch.h"
#include "CombinedConnection.h"

CombinedConnection::CombinedConnection(std::string start, std::string end)
	: Connection::Connection(start, end)
{
	FindAllConnections();
}

CombinedConnection::CombinedConnection()
{
	FindAllConnections();
}

void CombinedConnection::FindAllConnections()
{
	allConnections.FromFile("assets/databases/connections.csv");
	citiesIDs.FromDatabase(allConnections);
}

void CombinedConnection::FindStartEndConnections()
{
}
