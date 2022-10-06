#include"eq_solving.h"
#include<iostream>

constexpr auto GAMMA_0 = 5. / 3;
constexpr auto U_0 = 0.;
constexpr auto RHO_0 = 1.694 * 1e-4;
constexpr auto P_0 = 1.013 * 1e+6;

constexpr auto GAMMA_3 = 7. / 5;
constexpr auto U_3 = 1.229*1e+4;
constexpr auto C_3 = 3.6537 * 1e+4;
constexpr auto P_3 = 1.6768 * 1e+6;

Polynomial<double> init()
{
	int n = 2 * GAMMA_3 / (GAMMA_3 - 1);
	int alpha_0 = (GAMMA_0 + 1) / (GAMMA_0 - 1);
	auto RHO_3 = GAMMA_3 * P_3 / (C_3 * C_3);
	auto mu = (U_3 - U_0) * std::sqrt((GAMMA_0 - 1) * RHO_0 / (2*P_0));
	auto nu = 2 / (GAMMA_3 - 1) * std::sqrt(GAMMA_3 * (GAMMA_0 - 1) / 2 * RHO_0 / RHO_3 * P_3 / P_0);
	auto X = P_3 / P_0;

	std::vector<double> coeff(2*n+1);
	coeff[0] = X * X;
	coeff[n - 2] = -alpha_0 * nu * nu * X;
	coeff[n - 1] = 2 * alpha_0 * nu * (mu + nu) * X;
	coeff[n] = -(2 + (mu + nu) * (mu + nu) * alpha_0) * X;
	coeff[2 * n - 2] = -nu * nu;
	coeff[2 * n - 1] = 2 * nu * (mu + nu);
	coeff[2 * n] = -(mu + nu) * (mu + nu) + 1;
	std::reverse(coeff.begin(), coeff.end());

	return Polynomial<double>{ coeff };
}

int main()
{;
	auto pol= init();
}