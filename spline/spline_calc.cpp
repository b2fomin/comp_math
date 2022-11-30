#include"spline_calc.h"

std::vector<double> system_solving(const std::vector<std::vector<double>>& system)
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

std::vector<std::vector<double>> spline(const std::vector<std::vector<double>>& data)
{
	int N = data.size() - 1;
	if (N < 2) return std::vector<std::vector<double>>(0);
	std::vector<std::vector<double>> sol(N);
	for (int i = 0; i < N; ++i)
	{
		sol[i] = std::vector<double>(4);
		sol[i][0] = data[i + 1][1];
	}
	sol[N - 1][3] = 0;
	std::vector<std::vector<double>> system(N - 1);
	for (int i = 0; i < N - 1; ++i)
	{
		system[i] = std::vector<double>(4);
		system[i][0] = data[i + 1][0] - data[i][0];
		system[i][1] = 2 * (data[i + 1][0] -
			data[i][0] + data[i + 2][0] - data[i + 1][0]);
		system[i][2] = data[i + 2][0] - data[i + 1][0];
		system[i][3] = 3 * ((data[i + 2][1] - data[i + 1][1]) / (data[i + 2][0] - data[i + 1][0]) -
			(data[i + 1][1] - data[i][1]) / (data[i + 1][0] - data[i][0]));
	}
	system[0][0] = system[N - 2][2] = 0;
	auto c = system_solving(system);
	c.insert(c.begin(), 0);
	c.push_back(0);
	for (int i = 0; i < N; ++i)
	{
		sol[i][1] = (data[i + 1][1] - data[i][1]) /
			(data[i + 1][0] - data[i][0]) + (2 * c[i + 1] + c[i]) /
			3 * (data[i + 1][0] - data[i][0]);
		sol[i][2] = c[i + 1];
		sol[i][3] = (c[i + 1] - c[i]) / 3 / (data[i + 1][0] - data[i][0]);
	}

	return sol;
}