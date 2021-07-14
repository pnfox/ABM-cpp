#include "agents.h"

Firms::Firms(int nFirms) : numberOfFirms(nFirms)
{
	this->price = std::vector<float>(numberOfFirms, 0);
	for(int i=0; i < numberOfFirms; i++){
		this->price[i] = this->price_dist(this->gen);
	}
	this->debt = std::vector<float>(numberOfFirms, 0);
	this->networth = std::vector<float>(numberOfFirms, 0);
	this->profit = std::vector<float>(numberOfFirms, 0);
	this->interestRate = std::vector<float>(numberOfFirms, 0);
	this->leverage = std::vector<float>(numberOfFirms, 1);
	this->capital = std::vector<float>(numberOfFirms, 0);
	this->output = std::vector<float>(numberOfFirms, 0);
	this->lgdf = std::vector<float>(numberOfFirms, 0);
	this->defaulted = std::vector<float>(numberOfFirms, 0);
}

bool Firms::isDefaulted(int firm)
{
	return defaulted[firm] == 1 ? true : false;
}

Banks::Banks(int nBanks) : numberOfBanks(nBanks)
{
	this->badDebt = std::vector<float>(numberOfBanks, 0);
	this->networth = std::vector<float>(numberOfBanks, 10);
	this->profit = std::vector<float>(numberOfBanks, 0);
	this->interestRate = std::vector<float>(numberOfBanks, 0);
	this->deposit = std::vector<float>(numberOfBanks, 1);
	this->creditLinkDegree = std::vector<float>(numberOfBanks, 0);
	this->nonPerformingLoans = std::vector<float>(numberOfBanks, 0);
	this->defaulted = std::vector<float>(numberOfBanks, 0);

}

bool Banks::isDefaulted(int bank)
{
	return defaulted[bank] == 1 ? true : false;
}
