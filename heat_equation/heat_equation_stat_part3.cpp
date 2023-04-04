#include<iostream>
#include<vector>
#include<iomanip>
#include<cmath>
#include<functional>
#include<tuple>
#include<limits>

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

auto create_matrix_and_f_vec(std::vector<std::function<double(double)>>& kqf,
	double a0, double a1, double b0, double b1, std::size_t n)
{
	double h = 1. / (n - 1);
	std::vector<std::vector<double>> matrix(3, std::vector<double>(n));
	matrix[0][0] = -kqf[0](h / 2) - a0 * h; matrix[0][n - 1] = -kqf[0](1 - h / 2) - a1 * h;
	matrix[1][0] = kqf[0](h / 2); matrix[1][n - 1] = 0;
	matrix[2][0] = 0; matrix[2][n - 1] = kqf[0](1 - h / 2);

	std::vector<double> f_vec(n);
	f_vec[0] = -b0 * h; f_vec[n - 1] = -b1 * h;

	for (int i = 1; i < n - 1; ++i)
	{
		auto x = h * i;
		matrix[0][i] = -(kqf[0](x + h / 2) + kqf[0](x - h / 2)) - kqf[1](x) * h * h;
		matrix[1][i] = kqf[0](x + h / 2); matrix[2][i] = kqf[0](x - h / 2);
		f_vec[i] = -kqf[2](x) * h * h;
	}

	return std::make_tuple(matrix, f_vec);
}

int main()
{
	std::vector<std::function<double(double)>> kqf(3);
	for (int i = 0; i < 3; ++i)
	{
		std::vector<double> coeff(5);
		for (int j = 0; j < 5; ++j)
		{
			std::cin >> coeff[j];
		}
		kqf[i] = [coeff](double x) {return coeff[0] + coeff[1] * x +
			coeff[2] * std::sin(x) + coeff[3] * std::exp(-coeff[4] * x); };
	}
	double a0, a1, b0, b1;
	std::cin >> a0 >> b0 >> a1 >> b1;

	constexpr double eps = 1e-6;
	constexpr std::size_t m = 11;
	std::size_t n = 2;
	std::vector<double> u;
	bool stop = false;

	while (!stop)
	{
		n *= 2;
		auto tmp = create_matrix_and_f_vec(kqf, a0, a1, b0, b1, n);
		u = tridiag_solve(std::get<0>(tmp), std::get<1>(tmp));

		double ratio = static_cast<double>(u.size() - 1) / (m - 1);
		for (int i = 0; i < m; ++i)
		{
			auto j = static_cast<std::size_t>(i * ratio);
			if ((j != 0 && std::abs(u[j] - u[j - 1]) < eps) ||
				(j != u.size() - 1 && std::abs(u[j] - u[j + 1]) < eps))
			{
				stop = true; break;
			}
		}
	}

	std::cout << std::setprecision(6);

	double ratio = static_cast<double>(u.size() - 1) / (m - 1);
	for (int i = 0; i < m; ++i)
	{
		std::cout << u[static_cast<std::size_t>(i * ratio)] << " ";
	}

	return 0;
}