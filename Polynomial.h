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
	Polynomial(int n);
	Polynomial(std::vector<T>& coeff);
	template<typename... Args>
	Polynomial(Args... args);
	~Polynomial() = default;

public:
	int degree() const;
	template<typename U, typename F>
	friend Polynomial operator+(const Polynomial<U>& left, const Polynomial<F>& right);
};

template<typename T>
Polynomial<T>::Polynomial(std::vector<T>& coeff) : coeff{ coeff } {};

template<typename T>
Polynomial<T>::Polynomial(int n) :coeff{ std::vector<T>(n) } {};

template<typename T>
template<typename... Args>
Polynomial<T>::Polynomial(Args... args) : coeff{ ...args } {};

template<typename T>
int Polynomial<T>::degree() const { return coeff.size(); };

template<typename T, typename U>
Polynomial<decltype(T{} + U{}) > operator+(const Polynomial<T>& left, const Polynomial<U>& right)
{
	auto degree = std::max(left.degree(), right.degree());
	auto new_pol = Polynomial<decltype(T{} + U{}) > (degree);
	for (int i = 0; i < degree; ++i)
	{
		new_pol.coeff[i] = left.coeff[i] + right.coeff[i];
	}

	return new_pol;
}