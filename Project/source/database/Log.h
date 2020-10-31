#pragma once
#include <map>
#include <ostream>
#include <string>
#include "utility/UI.h"

const static char* ostreamTypeName = "class std::basic_ostream<char,struct std::char_traits<char> >";
const static char* ofstreamTypeName = "class std::basic_ofstream<char,struct std::char_traits<char> >";

// Contains info on single connection
struct Log {
	// Initializers

	// Empty object
	Log() = default;
	// Converts raw string from file to log
	Log(const std::string& line);
	// type (F - ferry, P - plane, T - train, M - mixed); mixed only for summary logs
	// !! IDs of cities must be updated individually !!
	Log(size_t ID, std::string carrier, std::string cityA, std::string cityB, float duration, float cost, char type, COLOR color = COLOR::WHITE)
		: ID(ID), carrier(carrier), cityA(cityA), cityB(cityB), duration(duration), cost(cost), type(type), color(color) {}
	
	// Others
	
	// Updates ID of cities using aliases from citiesIDs
	void UpdataIDsOfCities(const std::map<std::string, size_t>& S2I);
	// Checks if log matches filter
	bool FilterPass() const;
	friend std::ostream& operator<<(std::ostream& os, const Log& log);

	size_t ID = SIZE_MAX;
	std::string carrier = "";
	std::string cityA = "";
	std::string cityB = "";
	size_t cityA_ID = SIZE_MAX;
	size_t cityB_ID = SIZE_MAX;
	float duration = 0.f;
	float cost = 0.f;
	char type = '\0';
	// Used for coloring log's info while printing it out
	COLOR color = COLOR::WHITE;
};
