#include "pch.h"
#include "FlightConnection.h"

FlightConnection::FlightConnection(std::string start, std::string end)
	: Connection::Connection(start, end)
{
	FindAllConnections();
}

FlightConnection::FlightConnection()
{
	FindAllConnections();
}

void FlightConnection::FindAllConnections()
{
	allConnections.FromFile("assets/databases/connections.csv", 'F');
	citiesIDs.FromDatabase(allConnections);
}

void FlightConnection::FindStartEndConnections()
{

}
