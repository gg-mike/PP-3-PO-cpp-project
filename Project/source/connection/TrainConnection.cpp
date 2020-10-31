#include "pch.h"
#include "TrainConnection.h"

TrainConnection::TrainConnection() {
	InitDatabase();
}

void TrainConnection::InitDatabase() {
	database.Init("assets/databases/connections.csv", 'T');
}
