#include "pch.h"
#include "PlaneConnection.h"

PlaneConnection::PlaneConnection(){
	InitDatabase();
}

void PlaneConnection::InitDatabase() {
	database.Init("assets/databases/connections.csv", 'P');
}
