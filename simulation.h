#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <vector>
#include "agents.h"

class Simulation
{
	public:
		// parameters
		int time = 2000;
		int numberOfFirms;
		int numberOfBanks;
		float alpha = 0.2;
		float varpf = 0.4;
		float gamma = 0.02;
		int chi = 5;
		int lambd = 4;
		float adj = 0.1;
		int phi = 3;
		float beta = 0.7;
		float rCB = 0.02;
		float cB = 0.01;
		int seed = 10000;

		// Agents
		Firms firms;
		Banks banks;

		// Firm-bank adjacency matrix
		std::vector<std::vector<int>> link_fb;

		// Model outputs
		std::vector<float> GDP;
		std::vector<float> totalCapital;
		std::vector<float> avgFirmPrice;
		std::vector<float> totalWealth;
		std::vector<float> totalDebt;

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
		void reportResults(int p_time);
		void saveResults();
		void run();
};
#endif
