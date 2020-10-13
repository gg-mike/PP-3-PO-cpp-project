#pragma once
#include "utility/Graph.h"

class Connection
{
public:
	Connection(std::string start, std::string end) : start(start), end(end) {}
	Connection() = default;
	void UpdateInfo(std::string start, std::string end);
	Log ChooseConnection(int ID);
	inline void ShowAllConnections() { std::cout << allConnections; }
	inline void ShowStartEndConnections() { std::cout << startEndConnections; }
protected:
	virtual void FindAllConnections() = 0;
	virtual void FindStartEndConnections() = 0;

protected:
	std::string start;
	std::string end;
	Database allConnections;
	Database startEndConnections;
	CitiesIDs citiesIDs;
	Graph connectionsGraph;
};

