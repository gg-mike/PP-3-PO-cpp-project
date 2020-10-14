#include "pch.h"
#include "utility/UI.h"
#include "Reservation.h"

template<typename T>
void SpecificAdd(Reservation<T>& reservation) {
	int choice;
	bool isAdded = false;
	do {
		std::cout << Clr();
		system("cls");
		reservation.ShowConnections();
		choice = Input<int>("Which connections you want to add (-2 to abort)\n  ID: ");
		if (choice >= 0)
			isAdded = (reservation += std::array<size_t, 2>{ static_cast<size_t>(choice), 0 });
	} while (choice != -2 && !isAdded);
	if (isAdded)
		std::cout << "  Connection " << choice << " added.\n  ";
	else
		std::cout << "  Process aborted.\n  ";
	system("pause");
}

template<typename T>
void FindBetweenAdd(Reservation<T>& reservation) {
	std::string start, end;
	bool isChoosen = false;
	bool isAdded = false;
	bool first = true;
	do {
		if (!first) {
			std::cout << Clr(12) << "  Write a valid city name! ";
			system("pause");
		}
		std::cout << Clr();
		system("cls");
		reservation.ShowCities();
		start = Input<std::string>("From (-2 to abort): ");
		if (start != "-2" && start != "") {
			end = Input<std::string>("To   (-2 to abort): ");
			if (end != "-2" && end != "")
				isChoosen = true;
		}
		first = false;
	} while ((start != "-2" || end != "-2") && !isChoosen);
	if (isChoosen) {
		std::cout << "  Connection between " << Clr(6) << start << Clr() << " and " << Clr(6) << end << Clr() << ":\n";
		reservation.ShowConnections(start, end);
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
		menus(2, "Adding reservation");
		choice = Input<int>("Your choice: ");
		switch (choice) {
		case 1:
			// SPECIFIC
			SpecificAdd<T>(reservation);
			system("cls");
			break;
		case 2:
			// FIND BETWEEN
			FindBetweenAdd<T>(reservation);
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
	int choice;
	bool first = true;
	do {
		if (!first) {
			std::cout << Clr(12) << "  Write a valid number! ";
			system("pause");
		}
		std::cout << Clr();
		system("cls");
		reservation.ShowTickets();
		choice = Input<int>("Which reservations you want to delete (-2 to abort)\n  ID: ");
		first = false;
	} while (!reservation.IsValidReservationID(choice) && choice != -2);
	if (choice != -2) {
		std::cout << "  Are you sure you want to delete reservation nr" << choice << std::endl;
		std::cout << Clr(12) << "  This process is irreversible!\n" << Clr();
		if (std::tolower(Input<char>("y to confirm: ")) == 'y') {
			reservation -= (choice);
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
	int choice;
	do {
		menus(1, "Account Settings");
		choice = Input<int>("Your choice: ");
		switch (choice) {
		case 1:
			// SHOW
			system("cls");
			reservation.ShowTickets();
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
	int choice;
	do {
		menus(0, "Main menu");
		choice = Input<int>("Your choice: ");
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
		"assets/menus/add.menu"
	};
	Menus menus(menuFilepaths);
	MainLoop(menus);

	std::cout << "  Thank you for using my application.\n  ";
	system("pause");
}
