#pragma once
#include <vector>
#include "Log.h"
#include "Graph.h"

// Enables fast switching between city names and IDs
struct CitiesIDs {
	// Initializers

	// Empty object
	CitiesIDs() = default;
	// info from the database
	CitiesIDs(const std::vector<std::vector<Log>>& info) { Init(info); }
	// info from the database
	void Init(const std::vector<std::vector<Log>>& info);
	
	// Others

	friend std::ostream& operator<<(std::ostream& os, const CitiesIDs& citiesIDs);

	// City names to IDs
	std::map<std::string, size_t> S2I;
	// IDs to city names
	std::map<size_t, std::string> I2S;
	// List of all cities
	std::vector<std::string> cities;
};

// Contains all information extracted from the file
class Database {
public:
	// Initializers

	// Empty object
	Database() = default;
	// type (F - ferry, P - plane, M - mixed)
	Database(const std::string& filepath, char type = 'M') { Init(filepath, type); }
	// type (F - ferry, P - plane, M - mixed)
	void Init(const std::string& filepath, char type = 'M', bool showSummaries = false);
	// Initializing the database with info from a different one
	void Init(const std::vector<std::string>& header, std::vector<std::vector<Log>>& info, char type = 'M', bool showSummaries = false);

	// Getters

	// Gets the log with a given id
	Log GetLog(size_t ID) const;
	// Gets the block with a given id
	std::vector<Log> GetBlock(size_t ID) const;
	// Gets all logs which match the params, type (F - ferry, P - plane, T - train, M - mixed)
	std::vector<Log> GetLogs(const std::string& cityA, const std::string& cityB, char type = 'M') const;
	// Gets all logs which match the params, type (F - ferry, P - plane, T - train, M - mixed)
	std::vector<Log> GetLogs(size_t cityA_ID, size_t cityB_ID, char type = 'M') const;
	// Gets the header
	const std::vector<std::string>& GetHeader() const { return std::ref(header); }
	// Gets the summaries
	const std::vector<Log> GetSummaries() const { return std::ref(summaries); }
	// Gets info
	const std::vector<std::vector<Log>>& GetInfo() const { return std::ref(info); }
	// Gets the count of all logs in the database
	size_t GetLogCount() const { return logCount; }
	// Gets ref to Graph
	Graph& GetGraph() { return std::ref(graph); }
	// Gets const ref to CitiesIDs
	const CitiesIDs& GetCitiesIDs() const { return std::ref(citiesIDs); }

	// Others
	
	// Creates a summary for a given block, returns false if ID does not match any block
	// If 0 params given, a summary of the last block is created
	bool MakeSummary(size_t ID = SIZE_MAX);
	// Appends log to the last existing block
	void AppendLog(Log& log);
	// Adds a given block to the database
	void AddBlock(std::vector<Log>& block);
	// Erases the block with a given ID
	void EraseBlock(size_t ID);
	// Sorts blocks according to a given type (D - duration, C - cost)
	void SortBlock(char type);
	friend std::ostream& operator<<(std::ostream& os, const Database& database);

private:
	char type = '\0';
	// Title of log
	std::vector<std::string> header = { "ID", "Carrier", "From", "To", "Duration", "Cost", "Type" };
	// Summary logs
	std::vector<Log> summaries = {};
	// All logs in the database
	std::vector<std::vector<Log>> info = {};
	// Logs count in info
	size_t logCount = 0;
	// Showing summaries while printing
	bool showSummaries = false;

	// Aliases of cities
	CitiesIDs citiesIDs;
	// Graph of connections
	Graph graph;
};
