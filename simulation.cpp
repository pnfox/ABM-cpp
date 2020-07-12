#include <cmath>
#include "simulation.h"

Simulation::Simulation()
{
	
}

int Simulation::findBestBank(std::vector<int> potentialPartners)
{
	float bestInterest = INFINITY;
	float best = NAN;
	int i = banks.interestRate[0];
	for(int partner : potentialPartners){
		if(banks.interestRate[partner] < bestInterest){
			bestInterest = banks.interestRate[partner];
			best = partner;
		}
	}

	return best;
}

void Simulation::findMatchings(int p_time)
{

}

std::vector<int> Simulation::findBankCustomers(int bank)
{
	std::vector<int> customers;
	int index = 0;
	for(int i : link_fb[bank]){
		if(i != 0)
			customers.insert(customers.end(), index);
		index++;
	}
	return customers; 
}

void Simulation::calculateDeposits()
{

}

float Simulation::maxFirmWealth()
{
	float maxNetworth = -INFINITY;
	for(float net : firms.networth)
		if(net > maxNetworth)
			maxNetworth = net;
	return maxNetworth;
}

void Simulation::replaceDefaults()
{
	
}

void Simulation::updateInterestRates()
{
	std::vector<float> power(numberOfFirms, 0);
	for(int i=0; i < numberOfFirms; i++){
		power[i] = std::pow(banks.networth[i], -gamma);
		banks.interestRate[i] = gamma * power[i];
	}
}

void Simulation::updateFirmDebt()
{
	for(int i=0; i < numberOfFirms; i++){
		firms.debt[i] = firms.leverage[i] * firms.networth[i];
	}
}

void Simulation::updateFirmCapital()
{
	for(int i=0; i < numberOfFirms; i++){
		firms.capital[i] = firms.networth[i] + firms.debt[i];
	}
}

void Simulation::updateFirmOutput()
{
	std::vector<float> power(numberOfFirms, 0);
	for(int i=0; i < numberOfFirms; i++){
		power[i] = std::pow(firms.capital[i], beta);
		firms.output[i] = phi * power[i];
	}
}

void Simulation::updateFirmPrice()
{
	
}
