#include "pch.h"
#include "utility/UI.h"
#include "Reservation.h"

template<typename T>
void AddLoop(const Menus& menus, Reservation<T>& reservation) {
	system("cls");
	int choice;
	do {
		menus(2);
		choice = Input<int>("Your choice: ");
		switch (choice) {
		case 1:
			// SPECIFIC
			system("cls");
			break;
		case 2:
			// FIND BETWEEN
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
void DeleteLoop(const Menus& menus, Reservation<T>& reservation) {
	int choice;
	bool first = true;
	do {
		if (!first) {
			std::cout << Clr(12) << "Write a valid number! ";
			system("pause");
		}
		std::cout << Clr();
		system("cls");
		reservation.ShowTickets();
		choice = Input<int>("Which reservations you want to delete (-2 to exit): ");
		first = false;
	} while (!reservation.IsValidReservationNumber(choice) && choice != -2);

	std::cout << "Reservation " << choice << " deleted.\n";
}


template<typename T>
void AccountLoop(const Menus& menus) {
	system("cls");
	Reservation<T> reservation = Reservation<T>();
	int choice;
	do {
		menus(1);
		choice = Input<int>("Your choice: ");
		switch (choice) {
		case 1:
			// SHOW
			system("cls");
			reservation.ShowTickets();
			system("pause");
			break;
		case 2:
			// ADD
			system("cls");
			AddLoop<T>(menus, reservation);
			// TODO: Add tickets
			break;
		case 3:
			// DELETE
			system("cls");
			DeleteLoop<T>(menus, reservation);
			// TODO: Delete tickets
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
		menus(0);
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
	std::cout << "Thank you for using my application.\n";
	system("pause");
}
