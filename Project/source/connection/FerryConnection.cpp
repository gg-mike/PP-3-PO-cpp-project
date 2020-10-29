#include "pch.h"
#include "FerryConnection.h"

FerryConnection::FerryConnection() {
	InitDatabase();
}

void FerryConnection::InitDatabase() {
	database.Init("assets/databases/connections.csv", 'F');
}
