#include "pch.h"
#include "FlightConnection.h"

FlightConnection::FlightConnection(){
	InitDatabase();
}

void FlightConnection::InitDatabase() {
	database.Init("assets/databases/connections.csv", 'F');
}
