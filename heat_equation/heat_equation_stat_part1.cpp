#include<iostream>
#include<vector>
#include<iomanip>

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

	double u0, u1;
	std::cin >> u0 >> u1;

	std::size_t n; std::cin >> n;
	double h = 1. / (n - 1);

	std::vector<std::vector<double>> matrix(3, std::vector<double>(n));
	matrix[0][0] = matrix[0][n - 1] = 1;
	matrix[1][0]  = matrix[1][n - 1] = 0;
	matrix[2][0] = matrix[2][n - 1] = 0;

	std::vector<double> f_vec(n);
	f_vec[0] = u0; f_vec[n - 1] = u1;

	for (int i = 1; i < n - 1; ++i)
	{
		matrix[0][i] = -2 * k - q * h * h;
		matrix[1][i] = matrix[2][i] = k;
		f_vec[i] = -f * h * h;
	}

	auto u = tridiag_solve(matrix, f_vec);
	for (auto& elem : u) std::cout << std::setprecision(6) << elem << " ";

	return 0;
}