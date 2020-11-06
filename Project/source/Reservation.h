#pragma once
#include <memory>
#include "connection/FerryConnection.h"
#include "connection/PlaneConnection.h"
#include "connection/TrainConnection.h"
#include "connection/CombinedConnection.h"
#include "database/Database.h"

// Main class responsible for handling operations on the database
template <typename T>
class Reservation {
public:
	// Initializers

	// Inits connections and the database of reservations
	Reservation() {
		connections = std::make_shared<T>();
		InitReservationsDatabaseFile("assets/databases/reservations.csv");
		reservations.Init("assets/databases/reservations.csv", 'M', true);
	}

	// Manipulations on the reservations database

	// Adds reservations,
	// i[1]
	// 0 - single
	// 1 - single with switching dir
	// 2 - single append
	// 3 - single with switching dir append
	// 4 - block
	// !! Validation should be done in the application !!
	void operator+=(std::pair<size_t, size_t> i) {
		Log log;
		switch (i.second)
		{
		case 0:
			// Single
			log = connections->GetLogD(i.first);
			reservations.AddBlock(std::vector<Log>{ log });
			break;
		case 1:
			// Single with switching dir
			log = connections->GetLogD(i.first);
			std::swap(log.cityA, log.cityB);
			reservations.AddBlock(std::vector<Log>{ log });
			break;
		case 2:
			// Single append
			log = connections->GetLogD(i.first);
			reservations.AppendLog(log);
			break;
		case 3:
			// Single with switching dir append
			log = connections->GetLogD(i.first);
			std::swap(log.cityA, log.cityB);
			reservations.AppendLog(log);
			break;
		case 4:
			// Block
			std::vector<Log> block = connections->GetBlockSE(i.first);
			reservations.AddBlock(block);
			break;
		}
		if (i.second != 2 && i.second != 3)
			reservations.MakeSummary();
		else
			reservations.MakeSummary(reservations.GetInfo().size() - 1ull);
		SaveToFile("assets/databases/reservations.csv");
	}
	// Erases reservtions
	// !! Validation should be done in the application !!
	void operator-=(size_t i) {
		reservations.EraseBlock(i);
		SaveToFile("assets/databases/reservations.csv");
	}

	// Getters

	// Connections database

	// Gets the city name with a specific index from the connections database
	// Redirected to: std::string Connection::GetCityName(size_t)
	std::string GetCityNameCon(size_t ID) {
		return connections->GetCityName(ID);
	}
	// Gets the log with a given id from the connections database
	// Redirected to: Log Connection::GetLog(size_t)
	Log GetLogConD(size_t ID) {
		return connections->GetLogD(ID);
	}
	// Gets the block with a given id from the connections database
	// Redirected to: std::vector<Log> Connection::GetBlockSE(size_t)
	std::vector<Log> GetBlockConSE(size_t ID) {
		return connections->GetBlockSE(ID);
	}
	// Gets info from the connections database
	// Redirected to: const std::vector<std::vector<Log>>& Connection::GetInfo()
	const std::vector<std::vector<Log>>& GetInfoConD() {
		return connections->GetInfoD();
	}
	// Gets the count of all logs in the connections database
	// Redirected to: size_t Connection::GetLogCount()
	size_t GetLogCountConD() {
		return connections->GetLogCountD();
	}
	
	// Reservations database

	// Gets the log with a given id from the reservations database
	Log GetLogRes(size_t ID) {
		return reservations.GetLog(ID);
	}
	// Gets the block with a given id from the reservations database
	std::vector<Log> GetBlockRes(size_t ID) {
		return reservations.GetBlock(ID);
	}
	// Gets info from the reservations database
	// Redirected to: const std::vector<std::vector<Log>>& Connection::GetInfo()
	const std::vector<std::vector<Log>>& GetInfoRes() {
		return reservations.GetInfo();
	}
	// Gets the count of all logs in the reservations database
	size_t GetLogCountRes() {
		return reservations.GetLogCount();
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
	// Prints reservation
	void ShowReservation(size_t ID = SIZE_MAX) {
		std::cout << Border(0);
		if (ID == SIZE_MAX)
			ID = reservations.GetInfo().size() - 1ull;
		if (ID < reservations.GetInfo().size()) {
			std::cout << reservations.GetSummaries().at(ID) << std::endl;
			for (const auto& log : reservations.GetBlock(ID))
				std::cout << Border(1) << log << std::endl;
		}
		std::cout << Border(2);
	}
	// Prints the whole database of connections
	void ShowConnections() {
		connections->ShowDatabase();
	}
	// Print all options of connections found using pathfinding
	void ShowConnections(std::string start, std::string end, char type) {
		connections->Init(start, end, type);
		connections->ShowStartEndConnections();
	}

	// Others

	// Validates reservation ID
	bool IsValidBlockIDRes(size_t ID) {
		if (ID >= reservations.GetInfo().size())
			return false;
		return true;
	}
	// Validates the log ID in the connections database
	bool IsValidLogIDConD(size_t ID) {
		Log log = connections->GetLogD(ID);
		if (log.ID == SIZE_MAX || !log.FilterPass())
			return false;
		return true;
	}
	// Validates the block ID in the startEndConnections from the connections database
	bool IsValidBlockIDConSE(size_t ID) {
		if (ID >= connections->GetInfoSE().size())
			return false;
		return true;
	}
	// Validates the city choice
	bool IsValidCity(size_t cityID) {
		return connections->IsValidCity(cityID);
	}

private:
	// Creates a reservations file, if there is none
	void InitReservationsDatabaseFile(const std::string& filepath) {
		if (!std::filesystem::exists(filepath)) {
			std::ofstream ofs(filepath);
			ofs.close();
		}
	}
	// Saves reservations to a file
	void SaveToFile(const std::string& filepath) {
		std::ofstream ofs(filepath, std::ofstream::trunc);

		for (size_t i = 0; i < reservations.GetInfo().size(); i++) {
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
