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

std::ostream& operator<<(std::ostream& os, Border border)
{
	os << Clr(BClr);
	if (border.len == 0) {
		os << border.chars[border.mode % 3][0];
		for (int i = 0; i < columnWidths.size(); i++) {
			for (int w = 0; w < columnWidths[i] + 1; w++)
				os << (char)-51;
			if (border.mode < 3 && i != columnWidths.size() - 1)
				os << border.chars[border.mode % 3][1];
			else if (border.mode >= 3 && i != columnWidths.size() - 1)
				os << (char)-51;
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
// Menus
// ////////////////////

void Menus::FromFiles(const std::vector<std::string>& filepaths)
{
	for (int i = 0; i < filepaths.size(); i++) {
		std::string content = "";
		FromFile(filepaths[i], content);
		contents.push_back(content);
	}
}

void Menus::FromFile(const std::string& filepath, std::string& content)
{
	if (std::filesystem::exists(filepath)) {
		std::ifstream ifs(filepath);
		std::string line;
		while (std::getline(ifs, line))
			content += line + "\n";
		ifs.close();
	}
	else {
		std::cout << "Menu content is missing!\n"
			<< "Please reload the application with the new menu provided to this location: \"" << filepath << "\"\n"
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

bool IsInt(std::string line) {
	if (!line.size())
		return false;
	for (char c : line)
		if (!isdigit(c) && c != '-')
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

void WrongChoice(const std::string& print)
{
	std::cout << Clr(COLOR::RED) << print << " ";
	system("pause");
	std::cout << Clr();
}

std::string Time(float t) {
	std::string line = "";
	if (t < 10)
		line += "0" + std::to_string((int)t) + ":";
	else
		line += std::to_string((int)t) + ":";
	if ((t - std::trunc(t)) * 60.f < 10.f)
		line += "0" + std::to_string((int)(std::round((t - (int)t) * 60.f)));
	else
		line += std::to_string((int)(std::round((t - (int)t) * 60.f)));
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
	std::cout << print;
	std::string in;
	std::getline(std::cin, in);
	return in;
}

template<>
int Input<int>(const std::string& print) {
	std::cout << print;
	std::string in;
	std::getline(std::cin, in);
	if (IsInt(in))
		return std::stoi(in);
	else
		return INT_MAX;
}

template<>
size_t Input<size_t>(const std::string& print) {
	std::cout << print;
	std::string in;
	std::getline(std::cin, in);
	if (IsUInt(in))
		return std::stoull(in);
	else
		return SIZE_MAX;
}

template<>
double Input<double>(const std::string& print) {
	std::cout << print;
	std::string in;
	std::getline(std::cin, in);
	if (IsDouble(in))
		return std::stod(in);
	else
		return DBL_MAX;
}
