#include "pch.h"
#include "Graph.h"

Graph::Graph(const Database& database, int citiesCount, char type) : type(type)
{
	for (int i = 0; i < citiesCount; i++)
		nodes.push_back(Node(i, citiesCount));
	for (const auto& l : database.info) {
		nodes[l.cityA_ID].connectionsBit[l.cityB_ID] = true;
		nodes[l.cityB_ID].connectionsBit[l.cityA_ID] = true;
		nodes[l.cityA_ID].connectionsList.push_back(l.cityB_ID);
		nodes[l.cityB_ID].connectionsList.push_back(l.cityA_ID);
	}
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
