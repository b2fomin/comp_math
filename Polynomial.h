#pragma once
#include<vector>
#include<algorithm>

template<typename T>
class Polynomial
{
private:
	std::vector<T> coeff;
public:
	Polynomial() = default;
	Polynomial(int degree);
	Polynomial(std::vector<T>& coeff);
	template<typename... Args>
	Polynomial(Args... args);
	~Polynomial() = default;

public:
	int degree() const;
	template<typename U, typename F>
	friend Polynomial operator+(const Polynomial<U>& left, const Polynomial<F>& right);
	template<typename U, typename F>
	friend Polynomial operator*(const Polynomial<U>& left, const Polynomial<F>& right);
};

template<typename T>
Polynomial<T>::Polynomial(std::vector<T>& coeff) : coeff{ coeff } {};

template<typename T>
Polynomial<T>::Polynomial(int degree) :coeff{ std::vector<T>(n) } {};

template<typename T>
template<typename... Args>
Polynomial<T>::Polynomial(Args... args) : coeff{ ...args } {};

template<typename T>
int Polynomial<T>::degree() const { return coeff.size(); };

template<typename T, typename U>
Polynomial<decltype(T{} + U{}) > operator+(const Polynomial<T>& left, const Polynomial<U>& right)
{
	const Polynomial<decltype(T{} + U{}) > & pol_max, pol_min;
	if (left.degree() >= right.degree())
	{
		pol_max = left;
		pol_min = right;
	}
	else
	{
		pol_max = right;
		pol_min = left;
	}

	auto new_pol = Polynomial<decltype(T{} + U{}) > (pol_max.degree());
	int i = 0;
	for (i; i < pol_min.degree(); ++i)
	{
		new_pol.coeff[i] = pol_max.coeff[i] + pol_min.coeff[i];
	}

	for (i; i < pol_max.degree(); ++i)
	{
		new_pol.coeff[i] = pol_max.coeff[i];
	}

	return new_pol;
}

template<typename T, typename U>
Polynomial<decltype(T{} *U{}) > operator*(const Polynomial<T>& left, const Polynomial<U>& right)
{
	const Polynomial<decltype(T{} * U{}) > & pol_max, pol_min;
	if (left.degree() >= right.degree())
	{
		pol_max = left;
		pol_min = right;
	}
	else
	{
		pol_max = right;
		pol_min = left;
	}
	auto new_pol = Polynomial<decltype(T{} * U{}) > (pol_max.degree() + pol_min.degree());
	for (int i = 0; i < new_pol.degree(); ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			if (i - j >= pol_max.degree()) continue;
			if (j >= pol_min.degree()) break;
			new_pol.coeff[i] += pol_max.coeff[i - j] + pol_min[j];
		}
	}
}