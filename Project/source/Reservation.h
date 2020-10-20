#pragma once
#include <memory>
#include "connection/FlightConnection.h"
#include "connection/CruiseConnection.h"
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
		reservations.Init("assets/databases/reservations.csv");
	}


	// Manipulations on reservation's database

	// Adds reservations, i[1](0 - single, 1 - single with switching dir, 2 - block)
	// !! Validation should be done in application !!
	void operator+=(std::array<size_t, 2> i) {
		// Single
		if (i[1] == 0) {
			Log log = connections->GetLog(i[0]);
			reservations.AddBlock(std::vector<Log>{ log });
			reservations.MakeSummary();
			SaveToFile("assets/databases/reservations.csv");
		}
		// Single with switching dir
		else if (i[1] == 1) {
			Log log = connections->GetLog(i[0]);
			std::swap(log.cityA, log.cityB);
			reservations.AddBlock(std::vector<Log>{ log });
			reservations.MakeSummary();
			SaveToFile("assets/databases/reservations.csv");
		}
		// Block
		else {
			std::vector<Log> block = connections->GetBlock(i[0]);
			reservations.AddBlock(block);
			reservations.MakeSummary();
			SaveToFile("assets/databases/reservations.csv");
		}
	}
	// Erases reservtions
	// !! Validation should be done in application !!
	void operator-=(size_t i) {
		reservations.EraseBlock(i);
		SaveToFile("assets/databases/reservations.csv");
	}

	// Getters

	// Gets log with given id
	// Redirected to: Log Connection::GetLog(size_t)
	Log GetLog(size_t ID) {
		return connections->GetLog(ID);
	}
	// Gets block with given id
	// Redirected to: std::vector<Log> Connection::GetBlock(size_t)
	std::vector<Log> GetBlock(size_t ID) {
		return connections->GetBlock(ID);
	}
	// Gets count of all logs in database
	// Redirected to: size_t Connection::GetLogCount()
	size_t GetLogNumForAllConnections() {
		return connections->GetLogCount();
	}
	
	// Printers

	// Prints all cities
	void ShowCities() {
		connections->ShowAllCities();
	}
	// Prints all active reservations
	void ShowReservations() {
		std::cout << reservations << std::endl;
		std::cout << Clr(COLOR::DARK_GREY) << "  Number of reservations: " << reservations.GetInfo().size() << Clr() << std::endl;
	}
	// Prints whole database of connections
	void ShowConnections() {
		connections->ShowDatabase();
	}
	// Print all options of connections found using  pathfinding
	void ShowConnections(std::string start, std::string end, bool isWeighted, char type = 'D') {
		connections->Init(start, end, isWeighted, type);
		connections->ShowStartEndConnections();
	}

	// Others

	// Validates reservations ID
	bool IsValidReservationID(size_t ID) {
		if (reservations.GetLog(ID).ID == SIZE_MAX)
			return false;
		return true;
	}
	// Validates log ID in connections database
	bool IsValidConnectionID(size_t ID) {
		if (connections->GetLog(ID).ID == SIZE_MAX)
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
	bool IsValidCity(std::string city) {
		return connections->IsValidCity(city);
	}

private:
	// Creates reservations file with header, if there is none
	void InitReservationsDatabaseFile(const std::string& filepath) {
		if (!std::filesystem::exists(filepath)) {
			std::ofstream ofs(filepath);
			ofs << "ID;From;To;Duration;Cost;Mode";
			ofs.close();
		}
	}
	// Saves reservations to file
	void SaveToFile(const std::string& filepath) {
		std::ofstream ofs(filepath, std::ofstream::trunc);
		for (const auto& h : reservations.GetHeader())
			ofs << h << ';';
		ofs << std::endl;
		for (size_t i = 0; i < reservations.GetInfo().size(); i++) {
			if (i)
				ofs << 'B' << std::endl;
			for (const auto& log : reservations.GetInfo()[i])
				ofs << log << std::endl;
		}
		ofs << 'X' << std::endl;
		ofs.close();
	}

private:
	std::shared_ptr<Connection> connections;
	Database reservations;
};
