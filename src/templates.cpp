#include "../include/templates.h"
// define globals (one definition only)
std::string fileName;
std::ofstream sharedCSV;
// Motor definitions
Motor::Motor(std::string n): m_Name(std::move(n)) {}

std::vector<std::string> Motor::headers(const std::string& name) const {
	return {"VELOC", "POW"};
}

std::vector<int> Motor::read() const {
	return {67, 41};
}

// Color definitions
Color::Color(std::string n): m_Name(std::move(n)) {}

std::vector<std::string> Color::headers(const std::string& name) const {
	return {"HUE", "BRIGHT"};
}

std::vector<int> Color::read() const {
	return {1, 5};
}

