#include "pch.h"
#include "CruiseConnection.h"

CruiseConnection::CruiseConnection(std::string start, std::string end)
	: Connection::Connection(start, end)
{
	FindAllConnections();
}

CruiseConnection::CruiseConnection()
{
	FindAllConnections();
}

void CruiseConnection::FindAllConnections()
{
	allConnections.FromFile("assets/databases/connections.csv", 'C');
	citiesIDs.FromDatabase(allConnections);
}

void CruiseConnection::FindStartEndConnections()
{

}
