#pragma once
#include <map>
#include <ostream>
#include <string>

const static char* ostreamTypeName = "class std::basic_ostream<char,struct std::char_traits<char> >";
const static char* ofstreamTypeName = "class std::basic_ofstream<char,struct std::char_traits<char> >";

struct Log {
	Log(int ID, std::string cityA, std::string cityB, float duration, float cost, char type, int color = 15)
		: ID(ID), cityA(cityA), cityB(cityB), duration(duration), cost(cost), type(type), color(color) {}
	Log() : ID(-1), cityA(""), cityB(""), cityA_ID(-1), cityB_ID(-1), duration(-1), cost(-1), type('\0'), color(15) {}
	void UpdataIDs(std::map<std::string, int>& citiesS2I);
	friend std::ostream& operator<<(std::ostream& os, const Log& log);

	int ID;
	std::string cityA;
	std::string cityB;
	int cityA_ID = -1;
	int cityB_ID = -1;
	float duration;
	float cost;
	char type;
	int color;
};