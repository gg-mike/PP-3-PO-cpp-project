#pragma once
#include <array>
#include <ostream>
#include <string>
#include <vector>

constexpr auto BClr = 7;
extern std::array<int, 6> logWidths;

/*Values:
 1 - dark blue
 2 - dark green
 3 - dark cyan
 4 - dark red
 5 - dark pink
 6 - gold
 7 - light grey
 8 - dark grey
 9 - blue
10 - green
11 - cyan
12 - red
13 - pink
14 - yellow
15 - white
*/
struct Clr {
	Clr(char value=15) : value(value) {}
	friend std::ostream& operator<<(std::ostream& os, Clr clr)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr.value); 
		return os;
	}
	char value;
};

/* Modes:
0 - top    border for table of logs
1 - middle border for table of logs
2 - bottom border for table of logs
3 - top    border for table of menus
4 - middle border for table of menus
5 - bottom border for table of menus
*/
struct Border {
	Border(char mode) : mode(mode) {
		chars[0] = { -55, -53, -69 };
		chars[1] = { -52, -50, -71 };
		chars[2] = { -56, -54, -68 };
	}
	friend std::ostream& operator<<(std::ostream& os, Border border)
	{
		os << border.chars[border.mode % 3][0];
		for (int i = 0; i < logWidths.size(); i++) {
			for (int w = 0; w < logWidths[i] + 1; w++)
				os << (char)-51;
			if (border.mode < 3 && i != logWidths.size() - 1)
				os << border.chars[border.mode % 3][1];
			else if (border.mode >= 3 && i != logWidths.size() - 1)
				os << (char)-51;
			else
				os << border.chars[border.mode % 3][2] << std::endl;
		}
		return os;
	}
	char mode;
	std::array<std::array<char, 3>, 3> chars;
};

struct Menus {
	Menus(const std::vector<std::string>& filepaths) { FromFiles(filepaths); }
	void FromFiles(const std::vector<std::string>& filepaths);
	void FromFile(const std::string& filepath, std::string& content);
	std::string operator[](int i) { return contents[i]; }
	void operator()(int i) const { std::cout << Border(3) << contents[i] << Border(5) << std::endl; }

	std::vector<std::string> contents;
};

template <typename T> T Input(const std::string& print="");

// TODO: If user gives wrong number
inline void WrongChoice() {}