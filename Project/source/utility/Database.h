#pragma once
#include <vector>
#include "Log.h"

struct Database {
	Database(const std::string& filepath, char type='M') { FromFile(filepath, type); }
	Database() = default;
	int FindLog(const std::string& cityA, const std::string& cityB, char type);
	int FindLog(int cityA_ID, int cityB_ID, char type);
	void FromFile(const std::string& filepath, char type='M');
	void UpdataIDsInInfo(std::map<std::string, int>& citiesS2I);
	friend std::ostream& operator<<(std::ostream& os, const Database& database);

	std::vector<std::string> header;
	std::vector<Log> info;
	std::vector<int> splits;
};

struct CitiesIDs {
	CitiesIDs(const Database& database) { FromDatabase(database); }
	CitiesIDs() = default;
	void FromDatabase(const Database& database);
	friend std::ostream& operator<<(std::ostream& os, CitiesIDs& citiesIDs);

	std::map<std::string, int> dataS2I;
	std::map<int, std::string> dataI2S;
	std::vector<std::string> cities;
};