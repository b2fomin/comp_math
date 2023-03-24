#include<iostream>
#include<vector>
#include<iomanip>
#include<cmath>

std::vector<double> tridiag_solve(const std::vector<std::vector<double>>& matrix, const std::vector<double>& f)
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

int main()
{
	double k, q, f;
	std::cin >> k >> q >> f;

	double w, u;
	std::cin >> w >> w;

	std::size_t n, m;
	std::cin >> n >> m;
	double h = 1. / (n - 1);

	std::vector<std::vector<double>> matrix(3, std::vector<double>(n));
	matrix[0][0] = k; matrix[0][n - 1] = 1;
	matrix[1][0] = -k; matrix[1][n - 1] = 0;
	matrix[2][0] = 0; matrix[2][n - 1] = 0;

	std::vector<double> f_vec(n);
	f_vec[0] = w*h; f_vec[n - 1] = u;

	for (int i = 1; i < n - 1; ++i)
	{
		matrix[0][i] = -2 * k - q * h * h;
		matrix[1][i] = matrix[2][i] = k;
		f_vec[i] = -f * h * h;
	}

	auto u = tridiag_solve(matrix, f_vec);
	std::cout << std::setprecision(6);

	double ratio = static_cast<double>(u.size() - 1) / (m - 1);
	for (int i = 0; i < m; ++i)
	{
		std::cout << u[static_cast<std::size_t>(i * ratio)] << " ";
	}

	return 0;
}