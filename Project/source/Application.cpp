#include "pch.h"
#include "utility/UI.h"
#include "Reservation.h"

namespace UI {
	static bool running = true;
	Filter filter;
	static Guides guides;
	static Menus menus;
}

template<typename T>
void SpecificAdd(Reservation<T>& reservation) {
	std::string filterContent, citiesFilter = "";
	size_t choice;
	bool add = false;
	bool done = false;
	bool first = true;
	bool invalid = false;

	system("cls");
	bool smartFilter = Confirm("y to enable Smart Filter: ");
	system("cls");

	std::cout << "  Smart Filter: " << Clr(COLOR::DARK_YELLOW) << ((smartFilter) ? "enabled" : "disabled") << Clr() << std::endl;
	reservation.ShowConnections();

	while (!done) {
		if (!first && smartFilter)
			citiesFilter = reservation.GetBlockRes(reservation.GetBlockCountRes() - 1ull)[reservation.GetBlockRes(reservation.GetBlockCountRes() - 1ull).size() - 1].cityB;

		// Filter
		if (Confirm("y to enter database filter: ")) {
			filterContent = Input<std::string>(((first) ? "Filter (CCDCT): " : "Fliter (CDCT): "));
			UI::filter.Init(filterContent, ((first) ? "CCDCT" : "CDCT"));
			if (!first && smartFilter) {
				UI::filter.citiesActive = true;
				UI::filter.cities = citiesFilter;
			}
		}
		else if (!first && smartFilter) {
			UI::filter.Reset();
			UI::filter.active = true;
			UI::filter.citiesActive = true;
			UI::filter.cities = citiesFilter;
		}

		// Choosing connection
		do {
			if (invalid)
				WrongChoice("  Invalid connection ID!");
			system("cls");

			std::cout << "  Smart Filter: " << Clr(COLOR::DARK_YELLOW) << ((smartFilter) ? "enabled" : "disabled") << Clr() << std::endl;
			reservation.ShowConnections();

			std::cout << "  Which connections do you want to add?\n  You can switch direction of connection later.\n";
			choice = Input<size_t>("ID (0 to abort): ");
			if (choice && choice != SIZE_MAX)
				add = reservation.IsValidConnectionID(choice);

			invalid = true;
		} while (choice && !add);

		// Adding connection
		if (add) {
			// Switching direction of the connection
			if (Confirm("y to switch direction of connection: "))
				reservation += std::pair<size_t, size_t>{ choice, ((first) ? 1 : 3) };
			else
				reservation += std::pair<size_t, size_t>{ choice, ((first) ? 0 : 2) };


			if (!Confirm("y to add another connection: ")) {
				// Saving all changes
				if (Confirm("y to save all changes: "))
					std::cout << "  Route added.\n  ";
				// Discarding all changes
				else {
					std::cout << "  Are you sure you want to discard all changes?\n";
					std::cout << Clr(COLOR::RED) << "  This process is irreversible!" << Clr() << std::endl;

					if (Confirm("y to confirm: ")) {
						reservation -= (SIZE_MAX);
						std::cout << "  Changes discarded.\n  ";
					}
					else
						std::cout << "  Process aborted. Route added.\n  ";
				}
				done = true;
			}
		}
		else {
			done = true;
			std::cout << "  Process aborted.\n  ";
		}
		add = false;
		first = false;
		invalid = false;
	}
	system("pause");
}

template<typename T>
void FindBetweenAdd(Reservation<T>& reservation) {
	size_t startID = SIZE_MAX, endID = SIZE_MAX;
	bool isChoosen = false;
	bool isAdded = false;
	bool first = true;

	do {
		if (!first)
			WrongChoice();
		system("cls");

		std::cout << "  Cities\n";
		reservation.ShowCities();
		startID = Input<size_t>("From (0 to abort): ");
		if (startID && reservation.IsValidCity(startID)) {
			endID = Input<size_t>("To   (0 to abort): ");
			if (endID && reservation.IsValidCity(endID))
				isChoosen = true;
		}

		first = false;
	} while (startID && endID && !isChoosen);

	if (isChoosen) {
		isChoosen = false;
		size_t choice;
		std::string start = reservation.GetCityNameCon(startID - 1ull);
		std::string end = reservation.GetCityNameCon(endID - 1ull);
		char type = '\0';

		do {
			system("cls");

			std::cout << "  Connection between " << Clr(COLOR::DARK_YELLOW) << start << Clr() << " and " << Clr(COLOR::DARK_YELLOW) << end << Clr() << ".\n";
			UI::menus(3, "  Sort by:");
			choice = Input<size_t>("Your choice: ");

			switch (choice)
			{
			case 1:
				// Duration
				type = 'D'; break;
			case 2:
				// Cost
				type = 'C'; break;
			default:
				WrongChoice();
				break;
			}
		} while (type == '\0');

		size_t choice2;
		first = true;
		bool add = false;

		do {
			if (!first)
				WrongChoice("  Invalid connection ID!");
			system("cls");

			std::cout << "  Connection between " << Clr(COLOR::DARK_YELLOW) << start << Clr() << " and " << Clr(COLOR::DARK_YELLOW) << end << Clr() << std::endl;
			std::cout << "  Sorted by " << Clr(COLOR::DARK_YELLOW);
			switch (choice)
			{
			case 1: std::cout << "Duration"; break;
			case 2: std::cout << "Cost"; break;
			}
			std::cout << Clr() << std::endl;
			
			reservation.ShowConnections(start, end, type);
			std::cout << "  Which route do you want to add?\n";
			choice2 = Input<size_t>("ID (0 to abort): ");
			if (choice2 && choice2 != SIZE_MAX)
				add = reservation.IsValidRouteID(choice2 - 1ull);

			first = false;
		} while (choice2 && !add);
		if (choice2) {
			reservation += std::pair<size_t, size_t>{ choice2 - 1ull, 4 };
			std::cout << "  Route " << Clr(COLOR::DARK_YELLOW) << choice2 << Clr() << " added.\n  ";
		}
		else
			std::cout << "  Process aborted.\n  ";
	}
	else
		std::cout << "  Process aborted.\n  ";
	system("pause");
}

template<typename T>
void AddLoop(Reservation<T>& reservation) {
	system("cls");
	size_t choice = SIZE_MAX;
	
	while (UI::running && choice != 3){
		UI::menus(2, "  Adding reservation");
		choice = Input<size_t>("Your choice: ");
		switch (choice) {
		case 1:
			// Specific
			SpecificAdd<T>(reservation);
			system("cls");
			break;
		case 2:
			// Find between
			FindBetweenAdd<T>(reservation);
			system("cls");
			break;
		case 3:
			// Back
			break;
		case 4:
			// Help
			UI::guides(2);
			break;
		case 5:
			// Exit
			UI::running = false;
			break;
		default:
			WrongChoice();
			break;
		}
		system("cls");
	}
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
		std::cout << "  Which reservations do you want to delete?\n";
		choice = Input<size_t>("ID (0 to abort): ");

		first = false;
	} while (!reservation.IsValidReservationID(choice - 1ull) && choice);
	
	if (choice) {
		std::cout << "  Are you sure you want to delete reservation nr " << Clr(COLOR::DARK_YELLOW) << choice << Clr() << "?\n";
		std::cout << Clr(COLOR::RED) << "  This process is irreversible!" << Clr() << std::endl;
		
		if (Confirm("y to confirm: ")) {
			reservation -= (choice - 1ull);
			std::cout << "  Reservation " << Clr(COLOR::DARK_YELLOW) << choice << Clr() << " deleted.\n  ";
		}
		else
			std::cout << "  Process aborted.\n  ";
	}
	else
		std::cout << "  Process aborted.\n  ";
	system("pause");
}

template<typename T>
void AccountLoop() {
	Reservation<T> reservation = Reservation<T>();
	
	system("cls");
	size_t choice = SIZE_MAX;
	
	while (UI::running && choice != 4) {
		UI::menus(1, "  Account Settings");
		choice = Input<size_t>("Your choice: ");
		switch (choice) {
		case 1:
			// Show
			system("cls");
			reservation.ShowReservations();
			std::cout << "  ";
			system("pause");
			break;
		case 2:
			// Add
			AddLoop<T>(reservation);
			break;
		case 3:
			// Delete
			DeleteLoop<T>(reservation);
			break;
		case 4:
			// Back
			break;
		case 5:
			// Help
			UI::guides(1);
			break;
		case 6:
			// Exit
			UI::running = false;
			break;
		default:
			WrongChoice();
			break;
		}
		system("cls");
	}
}

void MainLoop() {
	size_t choice;
	while (UI::running) {
		UI::menus(0, "  Type of transport, you're interested in");
		choice = Input<size_t>("Your choice: ");
		switch (choice) {
		case 1:
			// Ferries
			AccountLoop<FerryConnection>();
			break;
		case 2:
			// Planes
			AccountLoop<PlaneConnection>();
			break;
		case 3:
			// Trains
			break;
		case 4:
			// All
			AccountLoop<CombinedConnection>();
			break;
		case 5:
			// Help
			UI::guides(0);
			break;
		case 6:
			// EXIT
			UI::running = false;
			break;
		default:
			WrongChoice();
			break;
		}
		system("cls");
	}
}

int main()
{
	std::vector<std::string> guideFilepaths = {
		"assets/helps/main.help",
		"assets/helps/account.help",
		"assets/helps/add.help"
	};
	std::vector<std::string> menuFilepaths = { 
		"assets/menus/main.menu", 
		"assets/menus/account.menu",
		"assets/menus/add.menu",
		"assets/menus/pathfinding.menu"
	};

	UI::guides.Init(guideFilepaths);
	UI::menus.Init(menuFilepaths);
	
	MainLoop();

	std::cout << "  Thank you for using my application.\n  ";
	system("pause");
}
