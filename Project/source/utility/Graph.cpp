#include "pch.h"
#include "Graph.h"

void Graph::FromDatabase(const Database& database, size_t citiesCount, char type)
{
	this->type = type;
	for (size_t i = 0; i < citiesCount; i++)
		nodes.push_back(Node(i, citiesCount));
	for	(const auto& block : database.info)
		for (const auto& log : block) {
			nodes[log.cityA_ID].connectionsBit[log.cityB_ID] = true;
			nodes[log.cityB_ID].connectionsBit[log.cityA_ID] = true;
			nodes[log.cityA_ID].connectionsList.push_back(log.cityB_ID);
			nodes[log.cityB_ID].connectionsList.push_back(log.cityA_ID);
		}
}

void Graph::FindConnections(const std::string& start, const std::string& end, std::vector<std::vector<RouteNode>>& results)
{
	results = {};
}

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
	switch (graph.type) {
	case 'F': os << "Flights:\n\n"; break;
	case 'C': os << "Cruises:\n\n"; break;
	case 'M': os << "Mixed:\n\n"; break;
	}
	for (const auto& r : graph.nodes) {
		for (const auto& b : r.connectionsBit)
			os << b;
		os << std::endl;
	}
	os << std::endl;
	for (const auto& r : graph.nodes) {
		os << r.index << ": ";
		for (const auto& l : r.connectionsList)
			os << l << " ";
		os << std::endl;
	}
	return os;
}
