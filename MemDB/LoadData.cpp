// This file is supposed to be compiled by the main analysis script
// This file is to be run via double-click by the user

#include <sstream>
#include <cstdlib>
#include "../Includes/ADSL.h"

int main() {
	std::cout << "Loading data..." << std::endl;
	adsl::createShared();

	// loop infinitely
	while (true) {}

}