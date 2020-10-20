#include "pch.h"
#include "Graph.h"

struct Edge {
	Edge() = delete;
	Edge(size_t start, size_t end, double weight)
		: start(start), end(end), weight(weight) {}
	friend bool operator<(const Edge& a, const Edge& b) {
		return a.weight < b.weight;
	}

	size_t start;
	size_t end;
	double weight;
};

void Graph::Init(const std::vector<std::vector<Log>>& info, size_t citiesCount) {
	graphNodes.clear();
	for (size_t i = 0; i < citiesCount; i++)
		graphNodes.push_back(Node());
	for	(const auto& block : info)
		for (const auto& log : block) {
			graphNodes[log.cityA_ID].connectionsSet.insert(log.cityB_ID);
			graphNodes[log.cityB_ID].connectionsSet.insert(log.cityA_ID);
		}
}

void Graph::InitMST(const std::vector<std::vector<Log>>& info, char type) {
	if (type != this->type) {
		this->type = type;
		for (const auto& block : info)
			for (const auto& log : block) {
				if (edgeWeights.find({ log.cityA_ID, log.cityB_ID }) == edgeWeights.end())
					edgeWeights[{ log.cityA_ID, log.cityB_ID }] = (type == 'D') ? log.duration : log.cost;
				else if (edgeWeights[{ log.cityA_ID, log.cityB_ID }] > ((type == 'D') ? log.duration : log.cost))
					edgeWeights[{ log.cityA_ID, log.cityB_ID }] = (type == 'D') ? log.duration : log.cost;
			}

		mstNodes.clear();
		for (size_t i = 0; i < graphNodes.size(); i++)
			mstNodes.push_back(Node());
		std::set<size_t> taken = { 0 };
		std::set<size_t> toTake = {};
		for (size_t i = 1; i < graphNodes.size(); i++)
			toTake.insert(i);

		while (toTake.size()) {
			std::vector<Edge> available;
			for (auto t : taken)
				for (auto s : graphNodes[t].connectionsSet)
					available.push_back(Edge(t, s, edgeWeights.at({ t, s })));

			std::sort(available.begin(), available.end());
			bool picked = false;
			size_t i = 0;
			while (!picked) {
				if (std::find(taken.begin(), taken.end(), available[i].end) == taken.end()) {
					mstNodes[available[i].start].connectionsSet.insert(available[i].end);
					mstNodes[available[i].end].connectionsSet.insert(available[i].start);
					taken.insert(available[i].end);
					toTake.erase(available[i].end);
					picked = true;
				}
				i++;
			}
		}
	}
}

void Graph::FindConnections(size_t startID, size_t endID, std::vector<size_t>& results) {
	FindConnectionUtility(graphNodes, startID, endID, results);
}

void Graph::FindConnectionsWeighted(size_t startID, size_t endID, std::vector<size_t>& results) {
	FindConnectionUtility(mstNodes, startID, endID, results);
}

void Graph::Reset(std::vector<Node>& nodes) {
	for (auto& n : nodes)
		n.visisted = false;
}

void Graph::FindConnectionUtility(std::vector<Node>& nodes, size_t startID, size_t endID, std::vector<size_t>& results) {
	bool routeFound = false;
	std::vector<size_t> con(nodes.size());
	con[startID] = SIZE_MAX;
	std::queue<size_t> points;
	points.push(startID);
	Reset(nodes);
	nodes[startID].visisted = true;
	while (!points.empty()) {
		size_t a = points.front();
		points.pop();
		if (a == endID) {
			routeFound = true;
			break;
		}
		for (auto& c : nodes[a].connectionsSet) {
			if (!nodes[c].visisted) {
				nodes[c].visisted = true;
				points.push(c);
				con[c] = a;
			}
		}
	}
	if (routeFound) {
		size_t i = endID;
		while (i != SIZE_MAX) {
			results.push_back(static_cast<size_t>(i));
			i = con[i];
		}
		std::reverse(results.begin(), results.end());
	}
}

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
	os << "Connections:" << std::endl;
	size_t i = 0;
	for (const auto& r : graph.graphNodes) {
		os << i << ": ";
		for (auto l : r.connectionsSet)
			os << l << " ";
		os << std::endl;
		i++;
	}
	i = 0;
	if (graph.type != '\0') {
		os << " MST:" << std::endl;
		for (const auto& r : graph.mstNodes) {
			for (auto l : r.connectionsSet)
				os << i << " -> " << l << " : " << graph.edgeWeights.at({ i, l }) << std::endl;
			os << std::endl;
			i++;
		}
	}

	return os;
}
