#include "pch.h"
#include "CruiseConnection.h"

CruiseConnection::CruiseConnection() {
	InitDatabase();
}

void CruiseConnection::InitDatabase() {
	database.Init("assets/databases/connections.csv", 'C');
}
