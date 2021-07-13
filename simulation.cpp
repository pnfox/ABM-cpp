#include <cmath>
#include <random>
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
        // Need an array of integers of length chi*numberofFirms
	// where each element represents the index of a bank
	// that could be a potential partner for a firm
	std::uniform_int_distribution<> dist(0, numberOfBanks);
	int bestBank = 0;
	int currentBank = 0;
	float newInterest = 0;
	float oldInterest = INFINITY;

	// TODO: For each firm do the following
	for(int f=0; f < numberOfFirms; f++){
		std::vector<int> potentialPartners(chi, 0);
		for(int i=0; i < chi; i++){
			potentialPartners[i] = dist(firms.gen);
		}

		bestBank = findBestBank(potentialPartners);

		newInterest = banks.interestRate[bestBank];

		// TODO: Get interest of old partner via link_fb
		for(int i=0; i < numberOfBanks; i++){
			currentBank = link_fb[f][i];
			if(currentBank != 0){
				oldInterest = banks.interestRate[currentBank];
				break;
			}
		}

		if(newInterest < oldInterest){
			// log change in firm-bank relationship
			// changeFB[p_time] = changeFB[p_time] + 1;

			// update link
			link_fb[f] = std::vector<int>(numberOfBanks);
			link_fb[f][bestBank] = 1;
		}
	}

	//changeFB[p_time] = changeFB[p_time] / numberOfFirms;
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
	int bankIndex = 0;
	std::vector<int> bankCustomers = {};
	float customersDebt = 0;
	float badDebt = 0;
	float bankProfit = 0;
	for(int i=0; i < numberOfBanks; i++){
		bankCustomers = {};
		customersDebt = 0;
		badDebt = 0;
		bankProfit = 0;
		bankCustomers = findBankCustomers(i);
		for(int j=0; j < bankCustomers.size(); j++){
			customersDebt += firms.debt[bankCustomers[j]];
		}
		banks.deposit[i] = customersDebt - banks.networth[i];

		// If bank has gone bankrupt
		if(banks.deposit[i] < 0){
			banks.deposit[i] = 0;
		}

		for(int j : bankCustomers){
			// compute bad debt
			if(firms.defaulted[j] == 1){
				badDebt += firms.lgdf[j] * firms.debt[j];
			} else if(firms.defaulted[j] == 0){
				bankProfit += firms.debt[j] * firms.interestRate[j];
			}
		}
		banks.badDebt[i] = badDebt;
		banks.profit[i] = bankProfit - (rCB * banks.deposit[i] - cB * banks.networth[i] - banks.badDebt[i]);
	}
}

float Simulation::getMaxFirmWealth()
{
	float maxNetworth = -INFINITY;
	for(float net : firms.networth)
		if(net > maxNetworth)
			maxNetworth = net;
	return maxNetworth;
}

void Simulation::replaceDefaults()
{
	std::uniform_int_distribution<> dist(0, numberOfBanks);
	std::uniform_real_distribution<> networth(0, 2);
	float maxFirmWealth = getMaxFirmWealth();
	int defaultedFirm;
	int defaultedBank;
	int newBankPartner;
	for(int i=0; i < numberOfFirms; i++){
		if(firms.defaulted[i] != 1)
			continue;
		defaultedFirm = i;

		// Zero the firm-bank links
		link_fb[defaultedFirm] = std::vector<int>(numberOfBanks);

		// Initialise the new partner link
		newBankPartner = dist(firms.gen);
		link_fb[defaultedFirm][newBankPartner] = 1;

		// Create new firms to replace defaulted ones
		firms.networth[defaultedFirm] = networth(firms.gen);
		firms.leverage[defaultedFirm] = 1;
		firms.price[defaultedFirm] = firms.price_dist(firms.gen);
		firms.interestRate[defaultedFirm] = rCB + banks.interestRate[newBankPartner] + \
						    + gamma * (firms.leverage[defaultedFirm] / \
							((1+firms.networth[defaultedFirm] / maxFirmWealth)));
		firms.defaulted[defaultedFirm] = 0;
	}

	// Replace defaulted Banks
	for(int i=0; i < numberOfBanks; i++){
		if(banks.defaulted[i] != 1)
			continue;
		defaultedBank = i;

		banks.networth[defaultedBank] = networth(firms.gen);
		banks.defaulted[defaultedBank] = 0;
	}


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
	firms.price_dist = std::normal_distribution<float>(alpha, std::sqrt(varpf));
	for(int i=0; i < numberOfFirms; i++){
		firms.price[i] = firms.price_dist(firms.gen);
	}
}

void Simulation::updateFirmInterestRate()
{
	float bestFirmWorth = getMaxFirmWealth();
	std::vector<int> banksOfFirms = {};
	for(int i=0; i < numberOfFirms; i++){
		for(int j=0; j < numberOfBanks; j++){
			if(link_fb[i][j] != 0){
				banksOfFirms.insert(banksOfFirms.end(), j);
			}
		}
	}
	//if(banksOfFirms.size() != numberOfFirms){
	//	std::cout << "Some firms missing banks" << std::endl;
	//}

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

void Simulation::updateFirmNetworth()
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

void Simulation::updateBankNetworth()
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
	std::uniform_real_distribution<double> dis(0,1);
	double u = 0;
	for(int i=0; i < numberOfFirms; i++){
		u = dis(firms.gen);
		if(firms.price[i] > firms.interestRate[i]){
			firms.leverage[i] = firms.leverage[i] * (1 + adj * u);
		} else{
			firms.leverage[i] = firms.leverage[i] * (1 - adj * u);
		}
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
}
