#ifndef _SIMULATION_H_
#define _SIMULATION_H_A

#include <vector>

class Simulation
{
	public:
		int time = 0;
		int numberOfFirms = 0;
		int numberOfBanks = 0;
		float alpha = 0.1;
		float varpf = 0.4;
		float gamma = 0.02;
		int chi = 5;
		int lambd = 4;
		float adj = 0.1;
		int phi = 3;
		float beta = 0.7;
		float rCB = 0.01;

		Simulation();

		int findBestBank(std::vector<int> potentialPartners);
		void findMatchings(int p_time);
		std::vector<int> findBankCustomers(int bank);
		void calculateDeposits();
		float maxFirmWealth();
		void replaceDefaults();
		void updateInterestRates();
		void updateFirmDebt();
		void updateFirmCapital();
		void updateFirmOutput();
		void updateFirmPrice();
		void updateFirmInterestRate();
		void updateFirmProfit();
		void updateFirmNetworth();
		void updateBankNetworth();
		void updateFirmLeverage();
		void updateFirmDebt();
		void updateLossRatio();
		void run();
};
#endif
