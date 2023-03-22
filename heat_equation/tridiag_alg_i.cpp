#include<iostream>
#include<vector>

double tridiag_solve_i(std::vector<std::vector<double>>& matrix, std::vector<double>& f, std::size_t i)
{
	--i;
	auto n = f.size();
	double a1 = 0, b1 = 0;
	for (std::size_t j = 0; j <= i; ++j)
	{
		auto a1_tmp = a1;
		a1 = -matrix[1][j] / (matrix[2][j] * a1_tmp + matrix[0][j]);
		b1 = (f[j] - matrix[2][j] * b1) / (matrix[2][j] * a1_tmp + matrix[0][j]);
	}

	double a2 = 0, b2 = 0;

	for (std::size_t j = n - 1; j > i; --j)
	{
		auto a2_tmp = a2;
		a2 = -matrix[2][j] / (matrix[0][j] + matrix[1][j] * a2_tmp);
		b2 = (f[j] - matrix[1][j] * b2) / (matrix[0][j] + matrix[1][j] * a2_tmp);
	}

	return (b1 + a1 * b2) / (1 - a2 * a1);
}

int main()
{
	std::size_t n;
	std::cin >> n;

	std::vector<std::vector<double>> matrix(3);
	matrix[0].resize(n);
	matrix[1].resize(n - 1);
	matrix[2].resize(n - 1);
	for (auto& elem : matrix)
	{
		for (int i = 0; i < elem.size(); ++i)
			std::cin >> elem[i];
	}

	matrix[2].insert(matrix[2].begin(), 0);
	matrix[1].push_back(0);

	std::vector<double> f(n);
	for (int i = 0; i < n; ++i) std::cin >> f[i];

	std::size_t i; std::cin >> i;

	std::cout << tridiag_solve_i(matrix, f, i) << std::endl;

	return 0;
}