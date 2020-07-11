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
		std::vector<float> debt(numberOfFirms, 0);
		std::vector<float> networth(numberOfFirms, 10);
		std::vector<float> profit(numberOfFirms, 0);
		std::vector<float> interestRate(numberOfFirms, 0);
		std::vector<float> leverage(numberOfFirms, 1);
		std::vector<float> capital(numberOfFirms, 0);
		std::vector<float> output(numberOfFirms, 0);
		std::vector<float> lgdf(numberOfFirms, 0);
		std::vector<float> defaulted(numberOfFirms, 0);

		Firms();

		bool isDefaulted(int firm);
};

class Banks
{
	public:
		int numberOfBanks = 10;
		std::vector<float> interestRate(numberOfBanks, 0);
		std::vector<float> networth(numberOfBanks, 10);
		std::vector<float> deposit(numberOfBanks, 0);
		std::vector<float> badDebt(numberOfBanks, 0);
		std::vector<float> profit(numberOfBanks, 0);
		std::vector<float> creditLinkDegree(numberOfBanks, 0);
		std::vector<float> nonPerformingLoans(numberOfBanks, 0);
		std::vector<float> defaulted(numberOfBanks, 0);

		Banks();

		bool isDefaulted(int bank);
};

#endif
