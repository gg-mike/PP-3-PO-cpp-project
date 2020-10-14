#pragma once
#include "Database.h"

struct RouteNode {
	RouteNode(int routeID, int cityA_ID, int cityB_ID)
		: routeID(routeID), cityA_ID(cityA_ID), cityB_ID(cityB_ID) {}

	int routeID;
	int cityA_ID;
	int cityB_ID;
};

struct Node
{
	Node(size_t i, size_t size)
		: index(i), visisted(false)
	{
		connectionsBit = std::vector<bool>(size);
		connectionsList = {};
	}

	size_t index;
	bool visisted;
	std::vector<bool> connectionsBit;
	std::vector<size_t> connectionsList;
};

class Graph {
public:
	Graph(const Database& database, size_t citiesCount, char type) { FromDatabase(database, citiesCount, type); }
	Graph() = default;
	void FromDatabase(const Database& database, size_t citiesCount, char type);
	void FindConnections(const std::string& start, const std::string& end, std::vector<std::vector<RouteNode>>& results);
	friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

private:
	char type = '\0';
	std::vector<Node> nodes = {};
};