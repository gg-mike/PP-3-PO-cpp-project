#pragma once
#include "database/Database.h"

// !! ABSTRACT CLASS !!
// Interface between the database and reservations
class Connection
{
public:
	// Initializers

	// Empty object
	Connection() = default;
	// Inits start and end locations and runs pathfinding
	// type(D - shortest duration, C - smallest cost)
	void Init(std::string start, std::string end, char type);

	// Getters

	// Gets the city name with a specific index
	std::string GetCityName(size_t ID);
	// Gets log with a given id from the database
	// Redirected to: Log Database::GetLog(size_t)
	Log GetLogD(size_t ID);
	// Gets block with a given id from startEndConnections
	// Redirected to: std::vector<Log> Database::GetBlock(size_t)
	std::vector<Log> GetBlockSE(size_t ID);
	// Gets info from the database
	// Redirected to: const std::vector<std::vector<Log>>& GetInfo()
	const std::vector<std::vector<Log>>& GetInfoD() const { return database.GetInfo(); }
	// Gets info from startEndConnections
	// Redirected to: const std::vector<std::vector<Log>>& GetInfo()
	const std::vector<std::vector<Log>>& GetInfoSE() const { return startEndConnections.GetInfo(); }
	// Gets count of all logs in the database
	// Redirected to: size_t Database::GetLogCount()
	size_t GetLogCountD() const { return database.GetLogCount(); }
	
	// Printers
	
	// Prints all cities using Database::citiesIDs
	void ShowAllCities();
	// Prints the whole database
	void ShowDatabase();
	// Print all options of connections found using pathfinding
	void ShowStartEndConnections();

	// Others

	// Validates the city choice
	bool IsValidCity(size_t cityID);
	// Validates the route ID
	bool IsValidRouteID(size_t ID);

protected:
	virtual void InitDatabase() = 0;
	// Converts pathfinding results into the database
	void PathToDatabase(const std::vector<size_t>& path, char type);
	// Pathfinding
	// type(D - shortest duration, C - smallest cost)
	void FindConnections(char type);

protected:
	std::string start = "";
	std::string end = "";
	Database database;
	Database startEndConnections;
};
