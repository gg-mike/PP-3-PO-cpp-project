#pragma once
#include "Database.h"

struct Node
{
	Node(int i, int size)
		: index(i), visisted(false)
	{
		connectionsBit = std::vector<bool>(size);
		connectionsList = std::vector<int>();
	}

	int index;
	bool visisted;
	std::vector<bool> connectionsBit;
	std::vector<int> connectionsList;
};

class Graph {
public:
	Graph(const Database& database, int citiesCount, char type);
	Graph() = default;
	// TODO : void FindConnection(const std::string& start, const std::string& end, bool direct, char preferedModeOfTransport);
	friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

private:
	char type;
	std::vector<Node> nodes;
};