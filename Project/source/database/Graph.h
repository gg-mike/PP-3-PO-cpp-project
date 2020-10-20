#pragma once
#include "Log.h"
#include <array>
#include <set>

// Stores all connections from given node and state of node
struct Node
{
	Node() = default;
	Node(std::set<size_t> connectionsSet)
		: connectionsSet(connectionsSet) {}

	bool visisted = false;
	std::set<size_t> connectionsSet = {};
};

// Representation of connections
class Graph {
public:
	// Initializers

	// Empty object
	Graph() = default;
	// info from database, citiesCount from citiesIDs
	Graph(const std::vector<std::vector<Log>>& info, size_t citiesCount) { Init(info, citiesCount); }
	// info from database, citiesCount from citiesIDs
	void Init(const std::vector<std::vector<Log>>& info, size_t citiesCount);
	// info from database, type (D - duration, C - cost)
	void InitMST(const std::vector<std::vector<Log>>& info, char type);

	// Pathfinding

	// Pathfinding in graphNodes
	void FindConnections(size_t startID, size_t endID, std::vector<size_t>& results);
	// Pathfinding in mstNodes
	void FindConnectionsWeighted(size_t startID, size_t endID, std::vector<size_t>& results);

	// Others

	friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

private:
	// Sets state of all nodes to false
	void Reset(std::vector<Node>& nodes);
	// Pathfinding for given nodes
	void FindConnectionUtility(std::vector<Node>& nodes, size_t startID, size_t endID, std::vector<size_t>& results);

private:
	// Type of weigth
	char type = '\0';
	std::map<std::set<size_t>, double> edgeWeights;
	// Used for pathfinding (smallest number of transers)
	std::vector<Node> graphNodes = {};
	// Used for pathfinding (smallest cost/duration)
	std::vector<Node> mstNodes = {};
};
