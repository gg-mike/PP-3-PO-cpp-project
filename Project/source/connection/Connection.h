#pragma once
#include "database/Database.h"

// !! ABSTRACT CLASS !!
// Interface between database and reservations
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

	// Gets city name with specify index
	std::string GetCityName(size_t ID);
	// Gets log with given id
	// Redirected to: Log Database::GetLog(size_t)
	Log GetLog(size_t ID);
	// Gets block with given id
	// Redirected to: std::vector<Log> Database::GetBlock(size_t)
	std::vector<Log> GetBlock(size_t ID);
	// Gets count of all logs in database
	// Redirected to: size_t Database::GetLogCount()
	size_t GetLogCount() const { return database.GetLogCount(); }
	// Gets count of all blocks in database
	// Redirected to: size_t Database::GetBlockCount()
	size_t GetBlockCount() const { return database.GetBlockCount(); }
	
	// Printers
	
	// Prints all cities using Database::citiesIDs
	void ShowAllCities();
	// Prints whole database
	void ShowDatabase();
	// Print all options of connections found using pathfinding
	void ShowStartEndConnections();

	// Others

	// Validates of city choice
	bool IsValidCity(size_t cityID);
	// Validates route ID
	bool IsValidRouteID(size_t ID);

protected:
	virtual void InitDatabase() = 0;
	// Converts results of pathfinding into the database
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
