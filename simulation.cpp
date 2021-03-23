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

void Simulation::updateFirmInterestRate()
{
	float bestFirmWorth = maxFirmWealth();
	std::vector<int> banksOfFirms = {};
	for(int i=0; i < numberOfFirms; i++){
		for(int j=0; j < numberOfBanks; j++){
			if(link_fb[i][j] != 0){
				banksOfFirms.insert(banksOfFirms.end(), j);
			}
		}
	}
	if(banksOfFirms.size() != numberOfFirms){
		std::cout << "Some firms missing banks" << std::endl;
	}

	for(int i=0; i < numberOfFirms; i++){
		firms.interestRate[i] = rCB + banks.interestRate[banksOfFirms[i]] + (gamma * firms.leverage[i]) / ((1+firms.networth[i])/bestFirmWorth);
	}
}

void Simulation::updateFirmProfit()
{
	for(int i=0; i < numberOfFirms; i++){
		firms.profit[i] = firms.price[i] * firms.output[i] - firms.interestRate[i] * firms.debt[i];
	}
}

void Simulation::updateFirmsNetworth()
{
	for(int i=0; i < numberOfFirms; i++){
		firms.networth[i] = firms.networth[i] + firms.profit[i];
		if(firms.networth[i] <= 0){
			firms.defaulted[i] = 1;
		} else{
			firms.defaulted[i] = 0;
		}
	}
}

void Simulation::updateBanksNetworth()
{
	for(int i=0; i < numberOfBanks; i++){
		banks.networth[i] = banks.networth[i] + banks.profit[i];
		if(banks.networth[i] <= 0){
			banks.defaulted[i] = 1;
		} else{
			banks.defaulted[i] = 0;
		}
	}
}

void Simulation::updateFirmLeverage()
{

}

void Simulation::updateFirmDebt()
{
	for(int i=0; i < numberOfFirms; i++){
		firms.debt[i] = firms.leverage[i] * firms.networth[i];
	}
}

void Simulation::updateLossRatio()
{
	for(int i=0; i < numberOfFirms; i++){
		firms.lgdf[i] = -firms.networth[i] / firms.debt[i];
		if(firms.lgdf[i] > 1){
			firms.lgdf[i] = 1;
		} else if(firms.lgdf[i] < 0){
			firms.lgdf[i] = 0;
	}
}
