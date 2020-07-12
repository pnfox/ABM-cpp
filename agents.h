#ifndef _AGENTS_H_
#define _AGENTS_H_

#include <vector>
#include <random>

class Firms
{
	public:
		int numberOfFirms = 100;
		float alpha = 0.1;
		float varpf = 0.4;
		std::vector<float> price;
		std::vector<float> debt = std::vector<float>(numberOfFirms, 0);
		std::vector<float> networth = std::vector<float>(numberOfFirms, 10);
		std::vector<float> profit = std::vector<float>(numberOfFirms, 0);
		std::vector<float> interestRate = std::vector<float>(numberOfFirms, 0);
		std::vector<float> leverage = std::vector<float>(numberOfFirms, 1);
		std::vector<float> capital = std::vector<float>(numberOfFirms, 0);
		std::vector<float> output = std::vector<float>(numberOfFirms, 0);
		std::vector<float> lgdf = std::vector<float>(numberOfFirms, 0);
		std::vector<float> defaulted = std::vector<float>(numberOfFirms, 0);

		Firms();

		bool isDefaulted(int firm);
};

class Banks
{
	public:
		int numberOfBanks = 10;
		std::vector<float> interestRate = std::vector<float>(numberOfBanks, 0);
		std::vector<float> networth = std::vector<float>(numberOfBanks, 10);
		std::vector<float> deposit = std::vector<float>(numberOfBanks, 0);
		std::vector<float> badDebt = std::vector<float>(numberOfBanks, 0);
		std::vector<float> profit = std::vector<float>(numberOfBanks, 0);
		std::vector<float> creditLinkDegree = std::vector<float>(numberOfBanks, 0);
		std::vector<float> nonPerformingLoans = std::vector<float>(numberOfBanks, 0);
		std::vector<float> defaulted = std::vector<float>(numberOfBanks, 0);

		Banks();

		bool isDefaulted(int bank);
};

#endif
