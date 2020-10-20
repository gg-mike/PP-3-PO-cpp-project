#include "pch.h"
#include "CombinedConnection.h"

CombinedConnection::CombinedConnection() {
	InitDatabase();
}

void CombinedConnection::InitDatabase() {
	database.Init("assets/databases/connections.csv");
}
