#pragma once
#include <ostream>
#include <string>
#include <array>
#include <set>
#include <vector>
#include <utility>

// !! For debug purposes only !!
#define K(x) std::cout << x << std::endl

// Used for colored prints
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
extern std::array<size_t, 7> columnWidths;

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
*/
struct Border {
	// Len if you want continous border with only one column
	Border(char mode, size_t len = 0) : mode(mode), len(len) {
		chars[0] = { -55, -53, -69 };
		chars[1] = { -52, -50, -71 };
		chars[2] = { -56, -54, -68 };
	}
	friend std::ostream& operator<<(std::ostream& os, Border border);
	char mode;
	size_t len;
	std::array<std::array<char, 3>, 3> chars;
};

// Filterer logs in database
struct Filter {
	Filter() = default;
	// Inits filter with given values
	// CCDCT: carriers;cities;duration;cost;type
	// CDCT : carriers;duration;cost;type
	void Init(const std::string& line, const std::string& filterType);
	// Resets all properties to the default values
	void Reset();
	friend std::ostream& operator<<(std::ostream& os, const Filter& filter);

	// active - is filter active
	// activeAttributeName - is filter of attribute is active
	// attributeName - filter for given attribute

	bool active = false;
	bool carriersActive = false;
	std::string carriers = "";
	bool citiesActive = false;
	std::string cities = "";
	bool durationActive = false;
	std::pair<double, double> duration = { 0, DBL_MAX };
	bool costActive = false;
	std::pair<double, double> cost = { 0, DBL_MAX };
	bool typesActive = false;
	std::string types = "";

	// If in duration or cost invalid values given, wrongFilters is used to display it
	bool wrongFiltersActive = false;
	std::string wrongFilters = "";
};

namespace UI {
	extern Filter filter;
}

// Handles loading and printing of help guides
struct Guides {
	Guides() = default;
	// Loads guides from all the given file paths
	Guides(const std::vector<std::string>& filepaths) { Init(filepaths); }
	void Init(const std::vector<std::string>& filepaths);
	virtual void Init(const std::string& filepath, std::string& content);
	virtual void operator()(int i) const;
	std::vector<std::string> contents;
};

// Handles loading and printing of menus
struct Menus : public Guides {
	Menus() = default;
	// Loads menus from all the given file paths
	Menus(const std::vector<std::string>& filepaths) { Guides::Init(filepaths); }
	void Init(const std::vector<std::string>& filepaths) { Guides::Init(filepaths); }
	void Init(const std::string& filepath, std::string& content) override;
	void operator()(int i) const override { std::cout << Border(3) << contents[i] << Border(5) << std::endl; }
	void operator()(int i, const std::string& print) const { 
		std::cout << Clr() << print << std::endl;
		std::cout << Border(3, 40) << contents[i] << Border(5, 40) << std::endl; 
	}
};

// String to lowercase
std::string ToLower(std::string line);
// Error message in red with pause
void WrongChoice(const std::string& print = "  Invalid input! ");
// Time formatting 
std::string Time(float t);
// Money formatting
std::string Money(float m);
// Safe input
template <typename T> 
T Input(const std::string& print = "");
// Used for confirming decisions
bool Confirm(const std::string& print = "", char confirmCh = 'y');
