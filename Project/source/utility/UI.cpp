#include "pch.h"
#include "UI.h"

// ////////////////////
// CLR
// ////////////////////

std::ostream& operator<<(std::ostream& os, Clr clr) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(clr.color));
	return os;
}

// ////////////////////
// BORDER
// ////////////////////

std::ostream& operator<<(std::ostream& os, Border border) {
	os << Clr(BClr);
	if (border.len == 0) {
		os << border.chars[border.mode % 3][0];
		for (int i = 0; i < columnWidths.size(); i++) {
			for (int w = 0; w < columnWidths[i] + 1; w++)
				os << (char)-51;
			if (i != columnWidths.size() - 1)
				os << border.chars[border.mode % 3][1];
			else
				os << border.chars[border.mode % 3][2] << std::endl;
		}
	}
	else {
		os << border.chars[border.mode % 3][0];
		for (int i = 0; i < border.len; i++)
			os << (char)-51;
		os << border.chars[border.mode % 3][2] << std::endl;
	}
	os << Clr();
	return os;
}

// ////////////////////
// Filter
// ////////////////////

void StringFilters(std::string& input, std::string& filters, bool& activation) {
	if (input.size() && input.find('*') == std::string::npos) {
		filters = input;
		activation = true;
	}
}

bool IsDouble(std::string line);

void PairFilters(std::string& input, std::pair<double, double>& filters, bool& activation, std::string& wrgFilters, bool& wrgFiltersActivation) {
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
	if (input.size() && input.find(',') != std::string::npos) {
		size_t i = 0;
		std::stringstream ss(input.c_str());
		std::string s;

		while (std::getline(ss, s, ',')) {
			if (s.size() && s[0] != '*') {
				if (IsDouble(s)) {
					if (!i)
						filters.first = std::stof(s);
					else
						filters.second = std::stof(s);
					activation = true;
				}
				else {
					wrgFilters += s + ' ';
					wrgFiltersActivation = true;
				}
			}
			i++;
		}
	}
	else if (input.size()) {
		wrgFilters += input + ' ';
		wrgFiltersActivation = true;
	}
}

void Filter::Init(const std::string& line, const std::string& filterType) {
	Reset();

	size_t i = 0;
	std::stringstream ss(line.c_str());
	std::string s;
	
	while (std::getline(ss, s, ';')) {
		// Skipping cities
		if (filterType == "CDCT" && i == 1)
			i = 2;

		switch (i)
		{
		case 0:
			// Company names
			StringFilters(s, carriers, carriersActive);
			break;
		case 1:
			// Cities
			StringFilters(s, cities, citiesActive);
			break;
		case 2:
			// Duration
			PairFilters(s, duration, durationActive, wrongFilters, wrongFiltersActive);
			break;
		case 3:
			// Cost
			PairFilters(s, cost, costActive, wrongFilters, wrongFiltersActive);
			break;
		case 4:
			// Type
			StringFilters(s, types, typesActive);
			break;
		}
		i++;
	}

	if (carriersActive || citiesActive || durationActive || costActive || typesActive || wrongFiltersActive)
		active = true;
}

void Filter::Reset() {
	active = false;
	carriersActive = false;
	carriers = "";
	citiesActive = false;
	cities = "";
	durationActive = false;
	duration = { 0, DBL_MAX };
	costActive = false;
	cost = { 0, DBL_MAX };
	typesActive = false;
	types = "";
	wrongFiltersActive = false;
	wrongFilters = "";
}

std::ostream& operator<<(std::ostream& os, const Filter& filter)
{
	if (filter.active) {
		os << "  Active filters:\n";
		if (filter.carriersActive)
			os << std::setw(20) << "Company names: " << Clr(COLOR::DARK_YELLOW) << filter.carriers << Clr() << std::endl;
		if (filter.citiesActive)
			os << std::setw(20) << "Cities: " << Clr(COLOR::DARK_YELLOW) << filter.cities << Clr() << std::endl;
		if (filter.durationActive) {
			os << std::setw(20) << "Duration: " << Clr(COLOR::DARK_YELLOW);
			os << filter.duration.first << " - ";
			if (filter.duration.second != DBL_MAX)
				os << filter.duration.second;
			os << Clr() << std::endl;
		}
		if (filter.costActive) {
			os << std::setw(20) << "Cost: " << Clr(COLOR::DARK_YELLOW);
			os << filter.cost.first << " - ";
			if (filter.cost.second != DBL_MAX)
				os << filter.cost.second;
			os << Clr() << std::endl;
		}
		if (filter.typesActive)
			os << std::setw(20) << "Types: " << Clr(COLOR::DARK_YELLOW) << filter.types << Clr() << std::endl;
		if (filter.wrongFiltersActive)
			os << std::setw(20) << "Invalid filers: " << Clr(COLOR::RED) << filter.wrongFilters << Clr() << std::endl;
	}
	else
		os << "  No active filters.\n";
	return os;
}

// ////////////////////
// Guides
// ////////////////////

void Guides::Init(const std::vector<std::string>& filepaths) {
	for (size_t i = 0; i < filepaths.size(); i++) {
		std::string content = "";
		Init(filepaths[i], content);
		contents.push_back(content);
	}
}

void Guides::Init(const std::string& filepath, std::string& content) {
	if (std::filesystem::exists(filepath)) {
		std::ifstream ifs(filepath);
		std::string line;

		while (std::getline(ifs, line))
			content += line + "\n";
		ifs.close();
	}
	else {
		std::cout << "Help guide content is missing!\n"
			<< "Please reload the application with the new menu provided to this location: " << Clr(COLOR::YELLOW) << filepath << Clr() << std::endl
			<< "Or make sure that given filepath is correct.\n";
		std::cin.get();
		exit(1);
	}
}

void Guides::operator()(int i) const {
	system("cls");
	std::string line;
	COLOR h1 = COLOR::WHITE;
	COLOR h2 = COLOR::LIGHT_GREY;
	COLOR impor = COLOR::DARK_YELLOW;
	COLOR plain = COLOR::DARK_GREY;
	COLOR clr = COLOR::DARK_GREY;

	for (auto c : contents[i])
		switch (c)
		{
		case '$':
			// H1
			clr = (clr == h1) ? plain : h1;
			std::cout << Clr(clr);
			break;
		case '#':
			// H2
			clr = (clr == h2) ? plain : h2;
			std::cout << Clr(clr);
			break;
		case '`':
			// Important information
			clr = (clr == impor) ? plain : impor;
			std::cout << Clr(clr);
			break;
		default:
			std::cout << c;
			break;
		}

	std::cout << Clr() << "\n  ";
	system("pause");
}


// ////////////////////
// Menus
// ////////////////////

void Menus::Init(const std::string& filepath, std::string& content) {
	if (std::filesystem::exists(filepath)) {
		std::ifstream ifs(filepath);
		std::string line;
		while (std::getline(ifs, line))
			content += line + "\n";
		ifs.close();
	}
	else {
		std::cout << "Menu content is missing!\n"
			<< "Please reload the application with the new menu provided to this location: " << Clr(COLOR::YELLOW) << filepath << Clr() << std::endl
			<< "Or make sure that given filepath is correct.\n";
		std::cin.get();
		exit(1);
	}
}

// ////////////////////
// Utilities
// ////////////////////

bool IsUInt(std::string line) {
	if (!line.size())
		return false;
	for (char c : line)
		if (!isdigit(c))
			return false;
	return true;
}

bool IsDouble(std::string line) {
	if (!line.size())
		return false;
	for (char c : line)
		if (!isdigit(c) && c != '.' && c != '-')
			return false;
	return true;
}

std::string ToLower(std::string line) {
	for (auto& c : line)
		c = tolower(c);
	return line;
}

void WrongChoice(const std::string& print) {
	std::cout << Clr(COLOR::RED) << print << " ";
	system("pause");
	std::cout << Clr();
}

std::string Time(float t) {
	std::string line = "";
	if (t < 10)
		line += "0" + std::to_string((int)t) + ':';
	else
		line += std::to_string((int)t) + ':';
	if ((t - std::trunc(t)) * 60 < 10)
		line += "0" + std::to_string((int)((t - (int)t) * 60));
	else 
		line += std::to_string((int)((t - (int)t) * 60));
	return line;
}

std::string Money(float m) {
	std::string line = std::to_string((int)m) + ".";
	int n = (int)(m - (int)m) * 100;
	if (n < 10)
		line += "0";
	line += std::to_string(n) + "$";
	return line;
}

// ////////////////////
// Input
// ////////////////////

template<>
std::string Input<std::string>(const std::string& print) {
	std::cout << std::setw(40) << print;
	std::string in;
	std::getline(std::cin, in);
	return in;
}

template<>
size_t Input<size_t>(const std::string& print) {
	std::cout << std::setw(40) << print;
	std::string in;
	std::getline(std::cin, in);
	in.erase(std::remove(in.begin(), in.end(), ' '), in.end());
	if (in.size() <= 18 && IsUInt(in))
		return std::stoull(in);
	else
		return SIZE_MAX;
}

bool Confirm(const std::string& print, char confirmCh) {
	std::string input = Input<std::string>(print);
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
	return std::tolower(input[0]) == confirmCh && input.size() == 1;
}
