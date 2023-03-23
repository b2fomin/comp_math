#include<iostream>
#include<vector>

std::vector<double> tridiag_solve(const std::vector<std::vector<double>>& matrix, const std::vector<double>& f)
{
	auto n = f.size();
	std::vector<std::vector<double>> coeff(5);
	for (int i = 0; i < 3; ++i) coeff[i].resize(n + 1);

	coeff[0][0] = coeff[1][0] = 0; coeff[2][0] = 1;
	for (int i = 0; i < n; ++i)
	{
		auto tmp = -(matrix[0][i] + matrix[2][i] * coeff[0][i]);
		coeff[0][i + 1] = matrix[1][i] / tmp;
		coeff[1][i + 1] = (-f[i] + matrix[2][i] * coeff[1][i]) / tmp;
		coeff[2][i + 1] = matrix[2][i] * coeff[2][i] / tmp;
	}

	coeff[3].resize(n - 1); coeff[4].resize(n - 1);
	coeff[3][n - 2] = coeff[1][n - 1];
	coeff[4][n - 2] = coeff[0][n - 1] + coeff[2][n - 1];

	for (int i = n - 2; i > 0; --i)
	{
		coeff[3][i - 1] = coeff[0][i] * coeff[3][i] + coeff[1][i];
		coeff[4][i - 1] = coeff[0][i] * coeff[4][i] + coeff[2][i];
	}

	std::vector<double> res(n);
	res[n - 1] = (coeff[1][n] + coeff[0][n] * coeff[3][0]) /
		(1 - coeff[0][n] * coeff[4][0] - coeff[2][n]);
	for (int i = n-2; i >=0; --i) res[i] = coeff[3][i] + res[n - 1] * coeff[4][i];
	return res;
}

int main()
{
	std::size_t n;
	std::cin >> n;

	std::vector<std::vector<double>> matrix(3, std::vector<double>(n));
	for (auto& elem : matrix)
	{
		for (int i = 0; i < elem.size(); ++i)
			std::cin >> elem[i];
	}

	std::vector<double> f(n);
	for (int i = 0; i < n; ++i) std::cin >> f[i];

	auto res = tridiag_solve(matrix, f);
	for (auto& elem : res) std::cout << elem << " ";

	return 0;
}