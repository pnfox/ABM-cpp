#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include "simulation.h"

Simulation::Simulation(int nFirms, int nBanks) : firms(nFirms), banks(nBanks)
{
	numberOfFirms = nFirms;
	numberOfBanks = nBanks;

	// Initialise firm-bank adjacency matrix
	std::vector<std::vector<int>> d(nFirms, std::vector<int>(nBanks));
	link_fb = d;
	std::uniform_int_distribution<> dist(0, numberOfBanks-1);
	for(int f=0; f < numberOfFirms; f++){
		link_fb[f][dist(this->firms.gen)] = 1;
		link_fb[f][dist(this->firms.gen)] = 1;
		link_fb[f][dist(this->firms.gen)] = 1;
		link_fb[f][dist(this->firms.gen)] = 1;
		link_fb[f][dist(this->firms.gen)] = 1;
	}

	GDP = std::vector<float>(time);
	totalCapital = std::vector<float>(time);
	avgFirmPrice = std::vector<float>(time);
	totalWealth = std::vector<float>(time);
	totalDebt = std::vector<float>(time);
}

int Simulation::findBestBank(std::vector<int> potentialPartners)
{
	float bestInterest = INFINITY;
	float best = NAN;
	for(int partner : potentialPartners){
		if(this->banks.interestRate[partner] < bestInterest){
			bestInterest = this->banks.interestRate[partner];
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
	std::uniform_int_distribution<> dist(0, numberOfBanks-1);
	int bestBank = 0;
	int currentBank = 0;
	float newInterest = 0;
	float oldInterest = INFINITY;
	std::vector<int> potentialPartners(chi, 0);

	for(int f=0; f < numberOfFirms; f++){
		for(int i=0; i < chi; i++){
			potentialPartners[i] = dist(this->firms.gen);
		}

		bestBank = findBestBank(potentialPartners);

		newInterest = this->banks.interestRate[bestBank];

		for(int i=0; i < numberOfBanks; i++){
			currentBank = link_fb[f][i];
			if(currentBank != 0){
				oldInterest = this->banks.interestRate[currentBank];
				break;
			}
		}

		if(newInterest < oldInterest){
			// log change in firm-bank relationship
			// changeFB[p_time] = changeFB[p_time] + 1;

			// update link
			link_fb[f][currentBank] = 0;
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
	int numberCustomers = 0;
	float customersDebt = 0;
	float badDebt = 0;
	float bankProfit = 0;
	for(int i=0; i < numberOfBanks; i++){
		bankCustomers = {};
		customersDebt = 0;
		badDebt = 0;
		bankProfit = 0;
		bankCustomers = findBankCustomers(i);
		numberCustomers = bankCustomers.size();
		for(int j=0; j < numberCustomers; j++){
			customersDebt += this->firms.debt[bankCustomers[j]];
		}
		this->banks.deposit[i] = customersDebt - this->banks.networth[i];

		// If bank has gone bankrupt
		if(this->banks.deposit[i] < 0){
			this->banks.deposit[i] = 0;
		}

		for(int j : bankCustomers){
			// compute bad debt
			if(this->firms.defaulted[j] == 1){
				badDebt += this->firms.lgdf[j] * this->firms.debt[j];
			} else if(this->firms.defaulted[j] == 0){
				bankProfit += this->firms.debt[j] * this->firms.interestRate[j];
			}
		}
		this->banks.badDebt[i] = badDebt;
		this->banks.profit[i] = bankProfit - (rCB * this->banks.deposit[i] - cB * this->banks.networth[i] - this->banks.badDebt[i]);
	}
}

float Simulation::getMaxFirmWealth()
{
	float maxNetworth = -INFINITY;
	for(float net : this->firms.networth)
		if(net > maxNetworth)
			maxNetworth = net;
	return maxNetworth;
}

void Simulation::replaceDefaults()
{
	std::uniform_int_distribution<> dist(0, numberOfBanks-1);
	std::uniform_real_distribution<> networth(0, 2);
	float maxFirmWealth = getMaxFirmWealth();
	int defaultedFirm;
	int defaultedBank;
	int newBankPartner;
	for(int i=0; i < numberOfFirms; i++){
		if(this->firms.defaulted[i] != 1)
			continue;
		defaultedFirm = i;

		// Zero the firm-bank links
		link_fb[defaultedFirm] = std::vector<int>(numberOfBanks);

		// Initialise the new partner link
		newBankPartner = dist(this->firms.gen);
		link_fb[defaultedFirm][newBankPartner] = 1;

		// Create new this->firms to replace defaulted ones
		this->firms.networth[defaultedFirm] = networth(this->firms.gen);
		this->firms.leverage[defaultedFirm] = 1;
		this->firms.price[defaultedFirm] = this->firms.price_dist(this->firms.gen);
		this->firms.interestRate[defaultedFirm] = rCB + this->banks.interestRate[newBankPartner] + \
						    + (gamma * this->firms.leverage[defaultedFirm]) / \
							((1+this->firms.networth[defaultedFirm] / maxFirmWealth));
		this->firms.defaulted[defaultedFirm] = 0;
	}

	// Replace defaulted Banks
	for(int i=0; i < numberOfBanks; i++){
		if(this->banks.defaulted[i] != 1)
			continue;
		defaultedBank = i;

		this->banks.networth[defaultedBank] = networth(this->firms.gen);
		this->banks.defaulted[defaultedBank] = 0;
	}


}

void Simulation::updateInterestRates()
{
	float power = 0;
	for(int i=0; i < numberOfBanks; i++){
		power = std::pow(this->banks.networth[i], -gamma);
		this->banks.interestRate[i] = gamma * power;
	}
}

void Simulation::updateFirmDebt()
{
	for(int i=0; i < numberOfFirms; i++){
		this->firms.debt[i] = this->firms.leverage[i] * this->firms.networth[i];
	}
}

void Simulation::updateFirmCapital()
{
	for(int i=0; i < numberOfFirms; i++){
		this->firms.capital[i] = this->firms.networth[i] + this->firms.debt[i];
	}
}

void Simulation::updateFirmOutput()
{
	float power = 0;
	for(int i=0; i < numberOfFirms; i++){
		power = std::pow(this->firms.capital[i], beta);
		this->firms.output[i] = phi * power;
	}
}

void Simulation::updateFirmPrice()
{
	this->firms.price_dist = std::normal_distribution<float>(alpha, std::sqrt(varpf));
	for(int i=0; i < numberOfFirms; i++){
		this->firms.price[i] = this->firms.price_dist(this->firms.gen);
	}
}

void Simulation::updateFirmInterestRate()
{
	float bestFirmWorth = getMaxFirmWealth();
	std::vector<int> banksOfFirms(numberOfFirms, 0);
	for(int i=0; i < numberOfFirms; i++){
		for(int j=0; j < numberOfBanks; j++){
			if(link_fb[i][j] != 0){
				banksOfFirms[i] = j;
			}
		}
	}

	for(int i=0; i < numberOfFirms; i++){
		this->firms.interestRate[i] = rCB + this->banks.interestRate[banksOfFirms[i]] + gamma * (this->firms.leverage[i]) / (1+this->firms.networth[i]/bestFirmWorth);
	}
}

void Simulation::updateFirmProfit()
{
	for(int i=0; i < numberOfFirms; i++){
		this->firms.profit[i] = this->firms.price[i] * this->firms.output[i] - this->firms.interestRate[i] * this->firms.debt[i];
	}
}

void Simulation::updateFirmNetworth()
{
	for(int i=0; i < numberOfFirms; i++){
		this->firms.networth[i] = this->firms.networth[i] + this->firms.profit[i];
		if(this->firms.networth[i] <= 0){
			this->firms.defaulted[i] = 1;
		} else{
			this->firms.defaulted[i] = 0;
		}
	}
}

void Simulation::updateBankNetworth()
{
	for(int i=0; i < numberOfBanks; i++){
		this->banks.networth[i] = this->banks.networth[i] + this->banks.profit[i];
		if(this->banks.networth[i] <= 0){
			this->banks.defaulted[i] = 1;
		} else{
			this->banks.defaulted[i] = 0;
		}
	}
}

void Simulation::updateFirmLeverage()
{
	std::uniform_real_distribution<double> dis(0,1);
	double u = 0;
	for(int i=0; i < numberOfFirms; i++){
		u = dis(this->firms.gen);
		if(this->firms.price[i] > this->firms.interestRate[i]){
			this->firms.leverage[i] = this->firms.leverage[i] * (1 + adj * u);
		} else{
			this->firms.leverage[i] = this->firms.leverage[i] * (1 - adj * u);
		}
	}
}

void Simulation::updateLossRatio()
{
	for(int i=0; i < numberOfFirms; i++){
		this->firms.lgdf[i] = -this->firms.networth[i] / this->firms.debt[i];
		if(this->firms.lgdf[i] > 1){
			this->firms.lgdf[i] = 1;
		} else if(this->firms.lgdf[i] < 0){
			this->firms.lgdf[i] = 0;
		}
	}
}

void Simulation::reportResults(int p_time)
{
	float totalOutput = 0;
	float capital = 0;
	float avgPrice = 0;
	float wealth = 0;
	float debt = 0;
	for(int f=0; f < numberOfFirms; f++){
		totalOutput += firms.output[f];
		capital += firms.capital[f];
		avgPrice += firms.price[f];
		wealth += firms.networth[f];
		debt += firms.debt[f];
	}
	GDP[p_time] = totalOutput;
	totalCapital[p_time] = capital;
	avgFirmPrice[p_time] = (avgPrice/(float)numberOfFirms);
	totalWealth[p_time] = wealth;
	totalDebt[p_time] = debt;
}

void Simulation::saveResults()
{
	std::ofstream outputFile;
	outputFile.open("output.csv");
	outputFile << "GDP,Firm Capital,Average Price,Total Wealth,Total Debt\n";
	for(int i=0; i < time; i++){
		outputFile << GDP[i] << ",";
		outputFile << totalCapital[i] << ",";
		outputFile << avgFirmPrice[i] << ",";
		outputFile << totalWealth[i] << ",";
		outputFile << totalDebt[i] << "\n";
	}
	outputFile.close();
}

void Simulation::run()
{
	for(int p_time=0; p_time < time; p_time++){
		updateInterestRates();

		findMatchings(p_time);

		updateFirmLeverage();

		updateFirmDebt();

		updateFirmCapital();

		updateFirmOutput();

		updateFirmPrice();

		updateFirmInterestRate();

		updateFirmProfit();

		updateFirmNetworth();

		updateLossRatio();

		calculateDeposits();

		updateBankNetworth();

		reportResults(p_time);

		replaceDefaults();

	}
	saveResults();
}
