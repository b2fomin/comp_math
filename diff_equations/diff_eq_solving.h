#pragma once
#include<vector>
#include<type_traits>

template<typename T>
std::vector<double> Runge_Kutta(T func, double x_0, double x_n, std::size_t count,
	double init_y, std::vector<std::vector<double>> Butcher_table)
{
	std::vector<double> y(count);
	y[0] = init_y;
	auto h = (x_n - x_0) / count;
	for (int k = 1; k < count; ++k)
	{
		auto x = x_0 + h * k;
		std::vector<double> f(Butcher_table.size() - 1);
		for (int i = 0; i < Butcher_table.size() - 1; ++i)
		{
			auto& row = Butcher_table[i];
			auto c = row[0];
			double a = 0;
			for (int j = 1; j < row.size(); ++j)
			{
				if (row[j]) a += row[j] * f[j - 1];
			}
			f[i] = func(x + c * h, y[k - 1] + a * h);
		}

		auto& f_coeff = Butcher_table[Butcher_table.size() - 1];
		y[k] = y[k - 1];

		for (int i = 1; i < f_coeff.size(); ++i)
		{
			y[k] += h * f_coeff[i] * f[i - 1];
		}
	}

	return y;
}

const std::vector<std::vector<double>> Heun_table
{
	{0,0,0,0},
	{1. / 3.,1. / 3.,0,0},
	{2. / 3.,0,2. / 3.,0},
	{0,1 / 4,0,3 / 4}
};