#pragma once
#include<vector>

auto system_solving(const std::vector<std::vector<double>>& system)
{
	std::vector<std::vector<double>> coeff;
	coeff.push_back(std::vector<double>(2));
	coeff[0][0] = system[0][2] / system[0][1];
	coeff[0][1] = system[0][3] / system[0][1];
	for (int i = 1; i < system.size(); ++i)
	{
		coeff.push_back(std::vector<double>(2));
		coeff[i][0] = system[i][2] /
			(system[i][1] - system[i][0] * coeff[i - 1][0]);
		coeff[i][1] = (system[i][3] - system[i][0] * coeff[i - 1][1]) /
			(system[i][1] - system[i][0] * coeff[i - 1][0]);
	}

	std::vector<double> sol(system.size());
	sol[sol.size() - 1] = coeff[coeff.size() - 1][1];
	for (int i = system.size() - 2; i >= 0; --i)
	{
		sol[i] = coeff[i][1] - coeff[i][0] * sol[i + 1];
	}

	return sol;
}
