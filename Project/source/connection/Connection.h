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
	void Init(std::string start, std::string end, bool isWeighted, char type = 'D');

	// Getters

	// Gets log with given id
	// Redirected to: Log Database::GetLog(size_t)
	Log GetLog(size_t ID);
	// Gets block with given id
	// Redirected to: std::vector<Log> Database::GetBlock(size_t)
	std::vector<Log> GetBlock(size_t ID);
	// Gets count of all logs in database
	// Redirected to: size_t Database::GetLogCount()
	size_t GetLogCount() const { return database.GetLogCount(); }
	
	// Printers
	
	// Prints all cities using Database::citiesIDs
	void ShowAllCities();
	// Prints whole database
	void ShowDatabase();
	// Print all options of connections found using pathfinding
	void ShowStartEndConnections();

	// Others

	// Valiadtion of city choice
	bool IsValidCity(std::string city);

protected:
	virtual void InitDatabase() = 0;
	// Converts results of pathfinding into the database
	void PathToDatabase(const std::vector<size_t>& path, bool isWeighted, char type);
	// Pathfinding
	// isWeighted(false) - smallest number of transers
	// isWeighted(true), type(D - smallest duration, C - smallest cost)
	void FindConnections(bool isWeighted, char type);

protected:
	std::string start = "";
	std::string end = "";
	Database database;
	Database startEndConnections;
};
