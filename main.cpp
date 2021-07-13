#include <algorithm>
#include <iostream>
#include "simulation.h"

int main(int argc, char* argv[])
{
	Simulation c;

	std::cout << "c.numberOfFirms: " << c.numberOfFirms << std::endl;
	std::cout << "c.numberOfBanks: " << c.numberOfBanks << std::endl;

	return 0;
}
