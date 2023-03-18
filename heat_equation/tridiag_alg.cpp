#include<iostream>
#include<vector>

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

	auto res = tridiag_solve(matrix, f);
	for (auto& elem : res) std::cout << elem << " ";

	return 0;
}