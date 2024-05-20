#include<iostream>
#include<vector>
#include<algorithm>
#include"func.h"

constexpr double eps = 5e-1;

std::vector<double> tridiag_solve(std::vector<std::vector<double>>& matrix, std::vector<double>& f)
{
	auto n = f.size();
	std::vector<std::vector<double>> coeff(2, std::vector<double>(n));
	coeff[0][0] = coeff[1][0] = 0;
	for (int i = 0; i < f.size() - 1; ++i)
	{
		coeff[0][i + 1] = -matrix[1][i] / (matrix[2][i] * coeff[0][i] + matrix[0][i]);
		coeff[1][i + 1] = (f[i] - matrix[2][i] * coeff[1][i]) / (matrix[2][i] * coeff[0][i] + matrix[0][i]);
	}

	std::vector<double> res(n);
	res[n - 1] = (f[n - 1] - matrix[2][n - 1] * coeff[1][n - 1]) / (matrix[0][n - 1] + matrix[2][n - 1] * coeff[0][n - 1]);
	for (int i = n - 1; i > 0; --i) res[i - 1] = coeff[0][i] * res[i] + coeff[1][i];

	return res;
}

auto calculate_u(double t, double tau, std::size_t L, std::size_t M,
	std::vector<std::vector<double>> u_old)
{
	double hx = 1. / (L - 1), hy = 1. / (M - 1);

	auto u = std::vector<std::vector<double>>(L, std::vector<double>(M));

	for (int m = 0; m < M; ++m)
	{
		double y = hy * m;
		std::vector<std::vector<double>> matrix(3, std::vector<double>(L, 0));
		std::vector<double> f(L, 0);

		matrix[0][0] = matrix[0][L - 1] = 1;
		f[0] = u_left(t, y);
		f[L - 1] = u_right(t, y);
		for (int i = 1; i < L - 1; ++i)
		{
			double x = hx * i;
			auto coeff1 = (k(t, x + hx, y, u_old[i + 1][m]) + k(t, x, y, u_old[i][m])) / (2 * hx * hx);
			auto coeff2 = (k(t, x, y, u_old[i][m]) + k(t, x - hx, y, u_old[i - 1][m])) / (2 * hx * hx);
			matrix[0][i] = 1 / tau + coeff1 + coeff2;
			matrix[1][i] = -coeff1;
			matrix[2][i] = -coeff2;
			f[i] = u_old[i][m] / tau;
		}
		auto u_tmp = tridiag_solve(matrix, f);
		for (auto& elem : u) elem[m] = u_tmp[m];
	}
	std::copy(u.begin(), u.end(), u_old.begin());
	for (int l = 0; l < L; ++l)
	{
		double x = hx * l;
		std::vector<std::vector<double>> matrix(3, std::vector<double>(M, 0));
		std::vector<double> f(M, 0);

		matrix[0][0] = matrix[0][M - 1] = 1;
		f[0] = u_bottom(t, x);
		f[f.size() - 1] = u_top(t, x);
		for (int i = 1; i < M - 1; ++i)
		{
			auto y = hy * i;
			auto coeff1 = (k(t, x, y + hy, u_old[l][i + 1]) + k(t, x, y, u_old[l][i])) / (2 * hy * hy);
			auto coeff2 = (k(t, x, y, u_old[l][i]) + k(t, x, y - hy, u_old[l][i - 1])) / (2 * hy * hy);
			matrix[0][i] = 1 / tau + coeff1 + coeff2;
			matrix[1][i] = -coeff1;
			matrix[2][i] = -coeff2;
			f[i] = u[l][i] / tau;
		}

		u[l] = tridiag_solve(matrix, f);
	}

	return u;
}

int main()
{
	std::size_t L, M;
	std::vector<std::vector<double>> u;
	double t = 0;
	while (t <= 1)
	{
		L = 10, M = 10;
		bool stop = false;
		double hx = 1. / (L - 1), hy = 1. / (M - 1);
		if (t == 0)
		{
			u = std::vector<std::vector<double>>(L, std::vector<double>(M));
			for (int i = 0; i < u.size(); ++i)
			{
				auto y = i * hy;
				for (int j = 0; j < u[0].size(); ++j)
				{
					auto x = j * hx;
					u[i][j] = u_init(x, y);
				}
			}
		}
		double tau = 0;
		while (true)
		{
			double hx = 1. / (L - 1), hy = 1. / (M - 1);
			auto u_old = std::vector<std::vector<double>>(L, std::vector<double>(M));
			for (int i = 0; i < u_old.size(); ++i)
			{
				for (int j = 0; j < u_old[0].size(); ++j)
				{
					u_old[i][j] = u[i / 2][j / 2];
				}
			}

			for (int i = 0; i < u_old.size(); ++i)
			{
				for (int j = 0; j < u_old[0].size(); ++j)
				{
					tau = std::max(tau, (pow(1. / (L - 1), 2) + pow(1. / (M - 1), 2)) /
						(2 * pow(k(t, j * hx, i * hy, u_old[i][j]), 2)));
					
				}
			}
			tau = std::min(tau, 1 / tau);
			double diff = 0;

			u = calculate_u(t, tau, L, M, u_old);
			for (int i = 0; i < u.size(); ++i)
			{
				for (int j = 0; j < u[0].size(); ++j)
				{
					diff = std::max(diff, std::abs(u_old[i][j] - u[i][j]) / u[i][j]);
				}
			}
			if (diff < eps)
			{
				break;
			}
			else
			{
				L *= 2; M *= 2;
			}
		}
		t += tau;

	}

	for (int i = 0; i < u.size(); i += u.size() / 20)
	{
		for (int j = 0; j < u[0].size(); j += u[0].size() / 20)
		{
			std::cout << static_cast<double>(i) / (L - 1) << '\t' <<
				static_cast<double>(j) / (M - 1) << '\t' << u[i][j] << std::endl;
		}
	}
	return 0;
}