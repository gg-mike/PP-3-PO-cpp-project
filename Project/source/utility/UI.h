#pragma once
#include <array>
#include <ostream>
#include <string>
#include <vector>

enum class COLOR {
	DARK_BLUE = 1,
	DARK_GREEN,
	DARK_CYAN,
	DARK_RED,
	DARK_PINK,
	DARK_YELLOW,
	LIGHT_GREY,
	DARK_GREY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PINK,
	YELLOW,
	WHITE
};

// Border's color
constexpr auto BClr = COLOR::LIGHT_GREY;
// Widths of column in logs printing
extern std::array<size_t, 6> columnWidths;

// Colores for prints
// Use enum COLOR
struct Clr {
	Clr(COLOR color = COLOR::WHITE) : color(color) {}
	friend std::ostream& operator<<(std::ostream& os, Clr clr);
	COLOR color;
};

// Borderes for prints
// Modes:
/*
0 - top    border for table of logs
1 - middle border for table of logs
2 - bottom border for table of logs
3 - top    border for table of menus
4 - middle border for table of menus
5 - bottom border for table of menus
*/
struct Border {
	// Len if you want continous border with only one column
	Border(char mode, int len = 0) : mode(mode), len(len) {
		chars[0] = { -55, -53, -69 };
		chars[1] = { -52, -50, -71 };
		chars[2] = { -56, -54, -68 };
	}
	friend std::ostream& operator<<(std::ostream& os, Border border);
	char mode;
	int len;
	std::array<std::array<char, 3>, 3> chars;
};


struct Menus {
	Menus(const std::vector<std::string>& filepaths) { FromFiles(filepaths); }
	void FromFiles(const std::vector<std::string>& filepaths);
	void FromFile(const std::string& filepath, std::string& content);
	std::string operator[](int i) { return contents[i]; }
	void operator()(int i) const { std::cout << Border(3) << contents[i] << Border(5) << std::endl; }
	void operator()(int i, const std::string& print) const { 
		std::cout << Clr() << print << std::endl;
		std::cout << Border(3) << contents[i] << Border(5) << std::endl; 
	}

	std::vector<std::string> contents;
};

// String to lowercase
std::string ToLower(std::string line);
// Error message in red with pause
void WrongChoice(const std::string& print = "  Invalid number! ");
// Time formatting 
std::string Time(float t);
// Money formatting
std::string Money(float m);
// Safe input
template <typename T> 
T Input(const std::string& print = "");

