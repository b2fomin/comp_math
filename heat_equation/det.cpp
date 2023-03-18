#include<iostream>
#include<vector>

double det(std::vector<std::vector<double>>& matrix)
{
	double res = matrix[0][0];
	double prev_res = 1;
	for (int i = 1; i < matrix[0].size(); ++i)
	{
		auto tmp = res;
		res = matrix[0][i] * res - matrix[2][i - 1] * matrix[1][i - 1] * prev_res;
		prev_res = tmp;
	}
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

	std::cout << det(matrix) << std::endl;

	return 0;
}