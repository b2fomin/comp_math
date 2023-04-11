#pragma once
#include<iostream>
#include<vector>
#include<limits>
#include<numeric>
#include<iomanip>
#include<cmath>
#include<fstream>
#include"func.h"

int main()
{
	constexpr double eps = 1e-2;
	constexpr int ratio = 2;
	std::size_t L = 2, N = 2;
	std::vector<double> u;
	bool stop = false;
	double norm_x, norm_t, prev_norm_x, prev_norm_t;
	norm_x = norm_t = std::numeric_limits<double>::max();

	while (!stop)
	{
		prev_norm_x = norm_x; norm_x = 0;
		prev_norm_t = norm_t; norm_t = 0;
		stop = true;

		u = std::vector<double>(L, 0);
		std::vector<double> vec_norm_x(N, 0), vec_norm_t(L, 0);
		double h = 1. / (L - 1);
		for (int i = 0; i < L; ++i)
		{
			u[i] = varphi(i * h);
			vec_norm_x[0] += u[i] * u[i];
			vec_norm_t[i] += u[i] * u[i];
		}

		for (int i = 0; i < N - 1; ++i)
		{
			double tau = 1. / (N - 1);
			auto t = i * tau;
			auto a_ij = std::numeric_limits<double>::min();
			for (int j = 0; j < L; ++j) a_ij = std::max(a_ij, std::abs(a(j * h, t)));
			tau = 2 * h / a_ij;
			for (int j = 0; j < L - 2; ++j)
			{
				auto x = j * h;
				a_ij = a(x, t);
				u[j] = u[j] + tau * tau * a_ij * a_ij * (u[j + 2] - 2 * u[j + 1] + u[j]) / (2 * h * h) -
					tau * (a_ij + tau / 2 * (a_t(x, t, tau) - a_ij * a_x(x, t, h))) * (4 * u[j + 1] - 3 * u[j] - u[j + 2]) / (2 * h) +
					tau * b(x, t) + tau * tau / 2 * (b_t(x, t, tau) - a_ij * b_x(x, t, h));
				vec_norm_t[j] += u[j] * u[j];
				vec_norm_x[i + 1] += u[j] * u[j];
			}

			u[L - 1] = psi(t);
			vec_norm_t[L - 1] += u[L - 1] * u[L - 1];

			u[L - 2] = u[L - 1] - h / a(1, t) * (b(1, t) - psi_1st(t, tau)) +
				h * h / (2 * a(1, t) * a(1, t)) * (psi_2nd(t, tau) + (b(1, t) - psi_1st(t, tau)) / a(1, t) *
					(a_t(1, t, tau) - a(1, t) * a_x(1, t, h)) - b_t(1, t, tau) + a(1, t) * b_x(1, t, h));
			vec_norm_t[L - 2] += u[L - 2] * u[L - 2];

			vec_norm_x[i + 1] += u[L - 1] * u[L - 1] + u[L - 2] * u[L - 2];
		}

		for (int i = 0; i < vec_norm_x.size(); ++i) norm_x += std::sqrt(vec_norm_x[i]);
		norm_x /= N*L;

		for (int i = 0; i < vec_norm_t.size(); ++i) norm_t += std::sqrt(vec_norm_t[i]);
		norm_t /= N*L;

		if (std::abs(prev_norm_x - norm_x) > eps) { L *= ratio; stop = false; }

		if (std::abs(prev_norm_t - norm_t) > eps) { N *= ratio; stop = false; }
	}

	double h = 1. / (u.size() - 1);
	std::fstream file("./output.txt", std::ios_base::out);
	file << std::setprecision(6);
	for (int i = 0; i < u.size(); ++i)
		file << h * i << '\t' << u[i] << std::endl;
	file.close();
}