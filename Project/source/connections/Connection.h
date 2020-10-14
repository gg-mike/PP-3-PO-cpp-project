#pragma once
#include "utility/Graph.h"

class Connection
{
public:
	Connection() = default;
	virtual void UpdateInfo(std::string start, std::string end);
	Log ChooseLog(size_t ID);
	std::vector<Log> ChooseBlock(size_t ID);
	size_t GetLogNum() const { return allConnections.logNum; }
	void ShowAllCities();
	void ShowAllConnections();
	void ShowStartEndConnections();
protected:
	friend void RouteNodesToLogs(const std::vector<std::vector<RouteNode>>& routeNodes, const Database& srcDatabase, CitiesIDs& citiesIDs, Database& destDatabase);
	virtual void FindAllConnections() = 0;
	virtual void FindStartEndConnections() = 0;

protected:
	std::string start = "";
	std::string end = "";
	Database allConnections;
	Database startEndConnections;
	CitiesIDs citiesIDs;
	Graph connectionsGraph;
};

