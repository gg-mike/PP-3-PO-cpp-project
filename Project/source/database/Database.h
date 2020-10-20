#pragma once
#include <vector>
#include "Log.h"
#include "Graph.h"

// Enables fast switching between cities names and IDs
struct CitiesIDs {
	// Initializers

	// Empty object
	CitiesIDs() = default;
	// info from Database
	CitiesIDs(const std::vector<std::vector<Log>>& info) { Init(info); }
	// info from Database
	void Init(const std::vector<std::vector<Log>>& info);
	
	// Others

	friend std::ostream& operator<<(std::ostream& os, const CitiesIDs& citiesIDs);

	// Cities names to IDs
	std::map<std::string, size_t> S2I;
	// IDs to cities names
	std::map<size_t, std::string> I2S;
	// List of all cities
	std::vector<std::string> cities;
};

// Contains all informations extracted from files
class Database {
public:
	// Initializers

	// Empty object
	Database() = default;
	// type (F - flights, C - cruises, M - mixed)
	Database(const std::string& filepath, char type = 'M') { Init(filepath, type); }
	// type (F - flights, C - cruises, M - mixed)
	void Init(const std::string& filepath, char type = 'M');
	// Initializing database with info from different one
	void Init(std::vector<std::vector<Log>>& info, char type = 'M');

	// Getters

	// Gets log with given id
	Log GetLog(size_t ID) const;
	// Gets block with given id
	std::vector<Log> GetBlock(size_t ID) const;
	// Gets all logs which match the params, type (F - flights, C - cruises, M - mixed)
	std::vector<Log> GetLogs(const std::string& cityA, const std::string& cityB, char type = 'M') const;
	// Gets all logs which match the params, type (F - flights, C - cruises, M - mixed)
	std::vector<Log> GetLogs(size_t cityA_ID, size_t cityB_ID, char type = 'M') const;
	// Gets header
	const std::vector<std::string>& GetHeader() const { return std::ref(header); }
	// Gets info
	const std::vector<std::vector<Log>>& GetInfo() const { return std::ref(info); }
	// Gets count of all logs in database
	size_t GetLogCount() const { return logCount; }
	// Gets ref to Graph
	Graph& GetGraph() { return std::ref(graph); }
	// Gets const ref to CitiesIDs
	const CitiesIDs& GetCitiesIDs() const { return std::ref(citiesIDs); }

	// Others
	
	// Creates summery for a given block, return false if ID is not matching any block
	// If 0 params given, summary of last block is created
	bool MakeSummary(size_t ID = SIZE_MAX);
	// Adds given block to database
	void AddBlock(std::vector<Log>& block);
	// Erases block with given ID
	void EraseBlock(size_t ID);
	friend std::ostream& operator<<(std::ostream& os, const Database& database);

private:
	char type = '\0';
	// Title of log
	std::vector<std::string> header = {};
	// Summary logs
	std::vector<Log> summaries = {};
	// All logs in database
	std::vector<std::vector<Log>> info = {};
	// Logs count in info
	size_t logCount = 0;

	// Aliases of cities
	CitiesIDs citiesIDs;
	// Graph of connections
	Graph graph;
};
