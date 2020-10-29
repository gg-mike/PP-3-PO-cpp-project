#include "pch.h"
#include "Graph.h"

void Graph::Init(const std::vector<std::vector<Log>>& info, size_t citiesCount) {
	nodes.clear();
	for (size_t i = 0; i < citiesCount; i++)
		nodes.push_back(Node());
	for (const auto& block : info)
		for (const auto& log : block) {
			// Nodes
			nodes[log.cityA_ID].connectionsSet.insert(log.cityB_ID);
			nodes[log.cityB_ID].connectionsSet.insert(log.cityA_ID);

			// Duration weights
			if (dWeights.find({ log.cityA_ID, log.cityB_ID }) == dWeights.end())
				dWeights[{ log.cityA_ID, log.cityB_ID }] = log.duration;
			else if (dWeights[{ log.cityA_ID, log.cityB_ID }] > log.duration)
				dWeights[{ log.cityA_ID, log.cityB_ID }] = log.duration;

			// Cost weights
			if (cWeights.find({ log.cityA_ID, log.cityB_ID }) == cWeights.end())
				cWeights[{ log.cityA_ID, log.cityB_ID }] = log.cost;
			else if (cWeights[{ log.cityA_ID, log.cityB_ID }] > log.cost)
				cWeights[{ log.cityA_ID, log.cityB_ID }] = log.cost;
		}
}


void Graph::FindConnections(size_t startID, size_t endID, char type, std::vector<size_t>& path) {
	std::vector<size_t> con(nodes.size());
	con[startID] = SIZE_MAX;
	con[endID] = SIZE_MAX;

	std::set<size_t> visited;
	std::queue<size_t> points;
	points.push(startID);
	Reset(nodes);

	nodes[startID].currWeight = 0.f;

	while (!points.empty()) {
		size_t a = points.front();
		points.pop();

		for (auto& c : nodes[a].connectionsSet) {
			double w = nodes[a].currWeight + ((type == 'D') ? dWeights.at({ a, c }) : cWeights.at({ a, c }));

			if (nodes[c].currWeight > w) {
				nodes[c].currWeight = w;
				con[c] = a;
			}

			if (visited.find(c) == visited.end()) {
				points.push(c);
				visited.insert(c);
			}
		}
	}

	if (con[endID] != SIZE_MAX) {
		size_t i = endID;

		while (i != SIZE_MAX) {
			path.push_back(i);
			i = con[i];
		}

		std::reverse(path.begin(), path.end());
	}
}

void Graph::Reset(std::vector<Node>& nodes) {
	for (auto& n : nodes)
		n.currWeight = DBL_MAX;

}

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
	os << "Connections:" << std::endl;
	size_t i = 0;
	for (const auto& n : graph.nodes) {
		os << i << ": ";
		for (auto c : n.connectionsSet)
			os << c << " ";
		os << std::endl;
		i++;
	}

	return os;
}
