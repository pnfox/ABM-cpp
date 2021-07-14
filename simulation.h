#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <vector>
#include "agents.h"

class Simulation
{
	public:
		// parameters
		int time = 0;
		int numberOfFirms = 100;
		int numberOfBanks = 10;
		float alpha = 0.1;
		float varpf = 0.4;
		float gamma = 0.02;
		int chi = 5;
		int lambd = 4;
		float adj = 0.1;
		int phi = 3;
		float beta = 0.7;
		float rCB = 0.02;
		float cB = 0.01;
		int seed = 900000;

		// Agents
		Firms firms;
		Banks banks;

		// Firm-bank adjacency matrix
		std::vector<std::vector<int>> link_fb;

		// Constructor
		Simulation(int nFirms, int nBanks);

		// Methods
		int findBestBank(std::vector<int> potentialPartners);
		void findMatchings(int p_time);
		std::vector<int> findBankCustomers(int bank);
		void calculateDeposits();
		float getMaxFirmWealth();
		void replaceDefaults();
		void updateInterestRates();
		void updateFirmDebt();
		void updateFirmCapital();
		void updateFirmOutput();
		void updateFirmPrice();
		void updateFirmInterestRate();
		void updateFirmProfit();
		void updateFirmNetworth();
		void updateFirmLeverage();
		void updateBankNetworth();
		void updateLossRatio();
		void run();
};
#endif
