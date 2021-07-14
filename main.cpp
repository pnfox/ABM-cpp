#include <algorithm>
#include <iostream>
#include "simulation.h"

int main(int argc, char* argv[])
{
	// Create a simulation with 100 firms and 10 banks
	Simulation c(100, 10);

	std::cout << "c.numberOfFirms: " << c.numberOfFirms << std::endl;
	std::cout << "c.numberOfBanks: " << c.numberOfBanks << std::endl;


	return 0;
}
