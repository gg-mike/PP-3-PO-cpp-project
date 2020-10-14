#pragma once
#include <vector>
#include "Log.h"

struct Database {
	Database(const std::string& filepath, char type='M') { FromFile(filepath, type); }
	Database() = default;
	Log GetLog(const std::string& cityA, const std::string& cityB, char type) const;
	Log GetLog(int cityA_ID, int cityB_ID, char type) const;
	Log GetLog(size_t ID) const;
	std::vector<Log> GetBlock(size_t ID);
	void MakeSummaryLog(size_t blockID = INT_MAX);
	void FromFile(const std::string& filepath, char type='M');
	void UpdataIDsInInfo(std::map<std::string, int>& citiesS2I);
	friend std::ostream& operator<<(std::ostream& os, const Database& database);

	std::vector<std::string> header = {};
	std::vector<Log> summaries = {};
	std::vector<std::vector<Log>> info = {};
	size_t logNum = 0;
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