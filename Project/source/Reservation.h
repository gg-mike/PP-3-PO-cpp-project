#pragma once
#include <memory>
#include "connections/FlightConnection.h"
#include "connections/CruiseConnection.h"
#include "connections/CombinedConnection.h"
#include "utility/Database.h"

template <typename T>
class Reservation {
public:
	Reservation() {
		connections = std::make_shared<T>();
		InitTicketsDatabase("assets/databases/reservations.csv");
		reservations.FromFile("assets/databases/reservations.csv");
	}

	// Displaying data

	void ShowTickets() {
		std::cout << reservations << std::endl;
		std::cout << Clr(8) << "  Number of reservations: " << reservations.info.size() << Clr() << std::endl;
	}
	void ShowCities() {
		connections->ShowAllCities();
	}
	void ShowConnections() {
		connections->ShowAllConnections();
	}
	void ShowConnections(std::string start, std::string end) {
		connections->UpdateInfo(start, end);
		connections->ShowStartEndConnections();
	}

	// Manipulations with data

	bool operator+=(std::array<size_t, 2> i) {
		// Single
		if (!i[1]) {
			Log log = connections->ChooseLog(i[0]);
			if (log.ID != -1) {
				reservations.info.push_back(std::vector<Log>{log});
				reservations.MakeSummaryLog();
				SaveToFile("assets/databases/reservations.csv");
				return true;
			}
			return false;
		}
		// Block
		else {
			std::vector<Log> block = connections->ChooseBlock(i[0]);
			if (block.size()) {
				reservations.info.push_back(block);
				reservations.MakeSummaryLog();
				SaveToFile("assets/databases/reservations.csv");
				return true;
			}
			return false;
		}
	}
	void operator-=(size_t i) {
		reservations.info.erase(reservations.info.begin() + i);
		SaveToFile("assets/databases/reservations.csv");
	}

	// Others
	Log ChooseLog(size_t ID) {
		return connections->ChooseLog(ID);
	}
	std::vector<Log> ChooseBlock(size_t ID) {
		return connections->ChooseBlock(ID);
	}
	size_t GetLogNumForAllConnections() {
		return connections->GetLogNum();
	}
	bool IsValidReservationID(size_t ID) {
		if (ID >= reservations.info.size())
			return false;
		return true;
	}
	bool IsValidConnectionID(size_t ID) {
		if (ID >= connections->GetLogNum())
			return false;
		return true;
	}

private:
	void InitTicketsDatabase(const std::string& filepath) {
		if (!std::filesystem::exists(filepath)) {
			std::ofstream ofs(filepath);
			ofs << "ID;From;To;Duration;Cost;Mode";
			ofs.close();
		}
	}
	void SaveToFile(const std::string& filepath) {
		std::ofstream ofs(filepath, std::ofstream::trunc);
		for (const auto& h : reservations.header)
			ofs << h << ';';
		ofs << std::endl;
		for (size_t i = 0; i < reservations.info.size(); i++) {
			if (i)
				ofs << 'B' << std::endl;
			for (const auto& log : reservations.info[i])
				ofs << log << std::endl;
		}
		ofs << 'X' << std::endl;
		ofs.close();
	}

private:
	std::shared_ptr<Connection> connections;
	Database reservations;
};