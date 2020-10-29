#pragma once
#include <memory>
#include "connection/PlaneConnection.h"
#include "connection/FerryConnection.h"
#include "connection/CombinedConnection.h"
#include "database/Database.h"

// Main class responsible for handling operations on database
template <typename T>
class Reservation {
public:
	// Initializers

	// Inits connections and database of reservations
	Reservation() {
		connections = std::make_shared<T>();
		InitReservationsDatabaseFile("assets/databases/reservations.csv");
		reservations.Init("assets/databases/reservations.csv", 'M', true);
	}

	// Manipulations on reservation's database

	// Adds reservations,
	// i[1]
	// 0 - single
	// 1 - single with switching dir
	// 2 - single append
	// 3 - single with switching dir append
	// 4 - block
	// !! Validation should be done in application !!
	void operator+=(std::pair<size_t, size_t> i) {
		Log log;
		switch (i.second)
		{
		case 0:
			// Single
			log = connections->GetLog(i.first);
			reservations.AddBlock(std::vector<Log>{ log });
			break;
		case 1:
			// Single with switching dir
			log = connections->GetLog(i.first);
			std::swap(log.cityA, log.cityB);
			reservations.AddBlock(std::vector<Log>{ log });
			break;
		case 2:
			// Single append
			log = connections->GetLog(i.first);
			reservations.AppendLog(log);
			break;
		case 3:
			// Single with switching dir append
			log = connections->GetLog(i.first);
			std::swap(log.cityA, log.cityB);
			reservations.AppendLog(log);
			break;
		case 4:
			// Block
			std::vector<Log> block = connections->GetBlock(i.first);
			reservations.AddBlock(block);
			break;
		}
		if (i.second != 2 && i.second != 3)
			reservations.MakeSummary();
		else
			reservations.MakeSummary(reservations.GetBlockCount() - 1);
		SaveToFile("assets/databases/reservations.csv");
	}
	// Erases reservtions
	// !! Validation should be done in application !!
	void operator-=(size_t i) {
		reservations.EraseBlock(i);
		SaveToFile("assets/databases/reservations.csv");
	}

	// Getters

	// Connections database

	// Gets city name with specify index from connections database
	// Redirected to: std::string Connection::GetCityName(size_t)
	std::string GetCityNameCon(size_t ID) {
		return connections->GetCityName(ID);
	}
	// Gets log with given id from connections database
	// Redirected to: Log Connection::GetLog(size_t)
	Log GetLogCon(size_t ID) {
		return connections->GetLog(ID);
	}
	// Gets block with given id from connections database
	// Redirected to: std::vector<Log> Connection::GetBlock(size_t)
	std::vector<Log> GetBlockCon(size_t ID) {
		return connections->GetBlock(ID);
	}
	// Gets count of all logs in connections database
	// Redirected to: size_t Connection::GetLogCount()
	size_t GetLogCountCon() {
		return connections->GetLogCount();
	}
	// Gets count of all blocks in connections database
	// Redirected to: size_t Connection::GetBlockCount()
	size_t GetBlockCountCon() {
		return connections->GetBlockCount();
	}
	
	// Reservations database

	// Gets log with given id from reservations database
	Log GetLogRes(size_t ID) {
		return reservations.GetLog(ID);
	}
	// Gets block with given id from reservations database
	std::vector<Log> GetBlockRes(size_t ID) {
		return reservations.GetBlock(ID);
	}
	// Gets count of all logs in reservations database
	size_t GetLogCountRes() {
		return reservations.GetLogCount();
	}
	// Gets count of all blocks in reservations database
	size_t GetBlockCountRes() {
		return reservations.GetBlockCount();
	}

	// Printers

	// Prints all cities
	void ShowCities() {
		connections->ShowAllCities();
	}
	// Prints all active reservations
	void ShowReservations() {
		std::cout << reservations << std::endl;
	}
	// Prints whole database of connections
	void ShowConnections() {
		connections->ShowDatabase();
	}
	// Print all options of connections found using  pathfinding
	void ShowConnections(std::string start, std::string end, char type) {
		connections->Init(start, end, type);
		connections->ShowStartEndConnections();
	}

	// Others

	// Validates reservations ID
	bool IsValidReservationID(size_t ID) {
		if (ID >= reservations.GetBlockCount())
			return false;
		return true;
	}
	// Validates route ID
	bool IsValidRouteID(size_t ID) {
		return connections->IsValidRouteID(ID);
	}
	// Validates log ID in connections database
	bool IsValidConnectionID(size_t ID) {
		Log log = connections->GetLog(ID);
		if (log.ID == SIZE_MAX || !log.FilterPass())
			return false;
		return true;
	}
	// Validates block ID in connection database
	bool IsValidBlockID(size_t ID) {
		if (connections->GetBlock(ID).size())
			return true;
		return false;
	}
	// Validates of city choice
	bool IsValidCity(size_t cityID) {
		return connections->IsValidCity(cityID);
	}

private:
	// Creates reservations file, if there is none
	void InitReservationsDatabaseFile(const std::string& filepath) {
		if (!std::filesystem::exists(filepath)) {
			std::ofstream ofs(filepath);
			ofs.close();
		}
	}
	// Saves reservations to file
	void SaveToFile(const std::string& filepath) {
		std::ofstream ofs(filepath, std::ofstream::trunc);

		for (size_t i = 0; i < reservations.GetBlockCount(); i++) {
			if (i)
				ofs << 'B' << std::endl;
			for (const auto& log : reservations.GetInfo()[i])
				ofs << log << std::endl;
		}

		ofs.close();
	}

private:
	std::shared_ptr<Connection> connections;
	Database reservations;
};
