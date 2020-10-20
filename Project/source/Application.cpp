#include "pch.h"
#include "utility/UI.h"
#include "Reservation.h"

template<typename T>
void SpecificAdd(Reservation<T>& reservation) {
	size_t choice;
	bool first = true;
	bool add = false;
	do {
		if (!first)
			WrongChoice("  Invalid connection ID!");
		system("cls");
		reservation.ShowConnections();
		choice = Input<size_t>("  Which connections you want to add\n  You can switch direction of connection later (0 to abort)\n  ID: ");
		if (choice && choice != SIZE_MAX)
			add = reservation.IsValidConnectionID(choice);
		first = false;
	} while (choice && !add);
	if (add) {
		if (std::tolower(Input<std::string>("  y to switch direction of connection: ")[0]) == 'y')
			reservation += std::array<size_t, 2>{ choice, 1 };
		else
			reservation += std::array<size_t, 2>{ choice, 0 };
		std::cout << "  Connection " << choice << " added.\n  ";
	}
	else
		std::cout << "  Process aborted.\n  ";
	system("pause");
}

template<typename T>
void FindBetweenAdd(const Menus& menus, Reservation<T>& reservation) {
	std::string start, end;
	bool isChoosen = false;
	bool isAdded = false;
	bool first = true;
	do {
		if (!first)
			WrongChoice("  Write a valid city name!");
		system("cls");
		std::cout << "  Cities\n";
		reservation.ShowCities();
		start = Input<std::string>("  From (0 to abort): ");
		if (start != "0" && reservation.IsValidCity(start)) {
			end = Input<std::string>("  To   (0 to abort): ");
			if (end != "0" && reservation.IsValidCity(end))
				isChoosen = true;
		}
		first = false;
	} while (start != "0" && end != "0" && !isChoosen);
	if (isChoosen) {
		isChoosen = false;
		size_t choice;
		bool isWeighted;
		char type = 'D';
		do {
			system("cls");
			std::cout << "  Connection between " << Clr(COLOR::DARK_YELLOW) << start << Clr() << " and " << Clr(COLOR::DARK_YELLOW) << end << Clr() << ".\n";
			menus(3, "  Sort by:");
			choice = Input<size_t>("  Your choice: ");

			switch (choice)
			{
			// TRANSFERS
			case 1:
				isChoosen = true;
				isWeighted = false;
				break;
			// DURATION
			case 2:
				isChoosen = true;
				isWeighted = true;
				break;
			// COST
			case 3:
				isChoosen = true;
				isWeighted = true;
				type = 'C';
				break;
			default:
				WrongChoice();
				break;
			}
		} while (!isChoosen);


		system("cls");
		std::cout << "  Connection between " << Clr(COLOR::DARK_YELLOW) << start << Clr() << " and " << Clr(COLOR::DARK_YELLOW) << end << Clr() << std::endl;
		std::cout << "  Sorted by " << Clr(COLOR::DARK_YELLOW);
		switch (choice)
		{		
		case 1: std::cout << "Number of transfers"; break;
		case 2: std::cout << "Duration"; break;
		case 3: std::cout << "Cost"; break;
		}
		std::cout << Clr() << std::endl;
		reservation.ShowConnections(start, end, isWeighted, type);


		std::cout << "  ";
	}
	else
		std::cout << "  Process aborted.\n  ";
	system("pause");
}

template<typename T>
void AddLoop(const Menus& menus, Reservation<T>& reservation) {
	system("cls");
	int choice;
	do {
		menus(2, "  Adding reservation");
		choice = Input<int>("  Your choice: ");
		switch (choice) {
		case 1:
			// SPECIFIC
			SpecificAdd<T>(reservation);
			system("cls");
			break;
		case 2:
			// FIND BETWEEN
			FindBetweenAdd<T>(menus, reservation);
			system("cls");
			break;
		case 3:
			// EXIT
			break;
		default:
			WrongChoice();
			break;
		}
		system("cls");
	} while (choice != 3);
}

template<typename T>
void DeleteLoop(Reservation<T>& reservation) {
	size_t choice;
	bool first = true;
	do {
		if (!first) 
			WrongChoice();
		system("cls");
		reservation.ShowReservations();
		choice = Input<size_t>("  Which reservations you want to delete (0 to abort)\n  ID: ");
		first = false;
	} while (!reservation.IsValidReservationID(choice - 1ull) && choice);
	if (choice) {
		std::cout << "  Are you sure you want to delete reservation nr" << choice << std::endl;
		std::cout << Clr(COLOR::RED) << "  This process is irreversible!" << Clr() << std::endl;
		if (Input<std::string>("  y to confirm: ") == "y") {
			reservation -= (choice - 1ull);
			std::cout << "  Reservation " << choice << " deleted.\n  ";
		}
		else
			std::cout << "  Process aborted.\n  ";
	}
	else
		std::cout << "  Process aborted.\n  ";
	system("pause");
}

template<typename T>
void AccountLoop(const Menus& menus) {
	system("cls");
	Reservation<T> reservation = Reservation<T>();
	size_t choice;
	do {
		menus(1, "  Account Settings");
		choice = Input<size_t>("  Your choice: ");
		switch (choice) {
		case 1:
			// SHOW
			system("cls");
			reservation.ShowReservations();
			std::cout << "  ";
			system("pause");
			break;
		case 2:
			// ADD
			system("cls");
			AddLoop<T>(menus, reservation);
			break;
		case 3:
			// DELETE
			system("cls");
			DeleteLoop<T>(reservation);
			break;
		case 4:
			// EXIT
			break;
		default:
			WrongChoice();
			break;
		}
		system("cls");
	} while (choice != 4);
}

void MainLoop(const Menus& menus) {
	size_t choice;
	do {
		menus(0, "  Main menu");
		choice = Input<size_t>("  Your choice: ");
		switch (choice) {
		case 1:
			// FLIGHTS
			AccountLoop<FlightConnection>(menus);
			break;
		case 2:
			// CRUISES
			AccountLoop<CruiseConnection>(menus);
			break;
		case 3:
			// BOTH
			AccountLoop<CombinedConnection>(menus);
			break;
		case 4:
			// EXIT
			break;
		default:
			WrongChoice();
			break;
		}
		system("cls");
	} while (choice != 4);
}

int main()
{
	std::vector<std::string> menuFilepaths = { 
		"assets/menus/main.menu", 
		"assets/menus/account.menu",
		"assets/menus/add.menu",
		"assets/menus/pathfinding.menu"
	};
	Menus menus(menuFilepaths);
	MainLoop(menus);
	

	std::cout << "  Thank you for using my application.\n  ";
	system("pause");
}
