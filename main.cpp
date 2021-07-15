#include <algorithm>
#include <iostream>
#include <chrono>
#include "simulation.h"

int main(int argc, char* argv[])
{
	// Create a simulation with 100 firms and 10 banks
	Simulation c(1000, 100);

	std::cout << "c.numberOfFirms: " << c.numberOfFirms << std::endl;
	std::cout << "c.numberOfBanks: " << c.numberOfBanks << std::endl;

	auto start = std::chrono::steady_clock::now();
	c.run();
	auto end = std::chrono::steady_clock::now();

	std::cout << "Time taken: " <<
	       std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()/1000.0	<< std::endl;

	return 0;
}
