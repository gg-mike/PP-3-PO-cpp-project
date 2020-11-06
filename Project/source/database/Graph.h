#pragma once
#include "Log.h"
#include <vector>
#include <set>

// Stores all connections from a given node and its state
struct Node
{
	Node() = default;
	Node(std::set<size_t> connectionsSet)
		: connectionsSet(connectionsSet) {}

	double currWeight = DBL_MAX;
	std::set<size_t> connectionsSet = {};
};

// Representation of connections
class Graph {
public:
	// Initializers

	// Empty object
	Graph() = default;
	// info from the database, citiesCount from citiesIDs
	Graph(const std::vector<std::vector<Log>>& info, size_t citiesCount) { Init(info, citiesCount); }
	// info from the database, citiesCount from citiesIDs
	void Init(const std::vector<std::vector<Log>>& info, size_t citiesCount);

	// Getters

	// Gets graph nodes
	const std::vector<Node>& GetNodes() const { return std::ref(nodes); }
	// Gets duration weights of connections
	const std::map<std::set<size_t>, double>& GetDWeights() const { return std::ref(dWeights); }
	// Gets cost weights of connections
	const std::map<std::set<size_t>, double>& GetCWeights() const { return std::ref(cWeights); }

	// Pathfinding

	// Dijkstra's algorithm, type(D - duration, C - cost)
	void FindConnections(size_t startID, size_t endID, char type, std::vector<size_t>& path);

	// Others

	friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

private:
	// Sets the state of all nodes to false
	void Reset(std::vector<Node>& nodes);

private:
	// Stores the smallest weight value for a given connection
	std::map<std::set<size_t>, double> dWeights;
	std::map<std::set<size_t>, double> cWeights;
	// Connection nodes
	std::vector<Node> nodes = {};
};
