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
		InitTicketsDatabase("assets/databases/tickets.csv");
		tickets.FromFile("assets/databases/tickets.csv");
	}

	// Displaying data

	void ShowTickets() {
		std::cout << tickets << std::endl;
		std::cout << std::endl << Clr(8) << "Number of tickets: " << tickets.splits.size() << Clr() << std::endl;
	}
	void ShowConnections() { 
		connections->ShowAllConnections(); 
	}
	void ShowConnections(std::string start, std::string end) {
		connections->UpdateInfo(start, end);
		connections->ShowStartEndConnections();
	}

	// Manipulations with data

	void operator+=(int num) {
		
	}
	void operator-=(int num) {

	}

	// Others
	void ChooseConnection(int ID) {
		connections->ChooseConnections(ID);
	}
	void ChooseMulipleConnections() {}
	bool IsValidReservationNumber(int n) {
		if (n >= tickets.splits.size())
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

private:
	std::shared_ptr<T> connections;
	Database tickets;
};