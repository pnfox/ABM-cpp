#ifndef _AGENTS_H_
#define _AGENTS_H_

#include <vector>
#include <random>

class Firms
{
	public:
		int numberOfFirms;
		float alpha = 0.1;
		float varpf = 0.4;
		unsigned int seed;
		std::mt19937 gen;
		std::vector<float> price;
		std::normal_distribution<float> price_dist{alpha, varpf};
		std::vector<float> debt;
		std::vector<float> networth;
		std::vector<float> profit;
		std::vector<float> interestRate;
		std::vector<float> leverage;
		std::vector<float> capital;
		std::vector<float> output;
		std::vector<float> lgdf;
		std::vector<float> defaulted;

		Firms(int nFirms);

		bool isDefaulted(int firm);
};

class Banks
{
	public:
		int numberOfBanks;
		std::vector<float> interestRate;
		std::vector<float> networth;
		std::vector<float> deposit;
		std::vector<float> badDebt;
		std::vector<float> profit;
		std::vector<float> creditLinkDegree;
		std::vector<float> nonPerformingLoans;
		std::vector<float> defaulted;

		Banks(int nBanks);

		bool isDefaulted(int bank);
};
#endif
