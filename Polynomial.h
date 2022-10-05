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
	Polynomial(std::size_t degree);
	Polynomial(std::vector<T>& coeff);
	Polynomial(std::initializer_list<T>);
	template<typename... Args>
	Polynomial(Args... args);
	template<typename U>
	Polynomial(const Polynomial<U>&);
	template<typename U>
	Polynomial<T>& operator=(const Polynomial<U>&);
	~Polynomial() = default;
private:
	void true_degree();
public:
	std::size_t degree() const;
	const std::vector<T>& coeff_() const;
	template<typename U>
	decltype(T{}*U{}) operator()(U& value) const;
	Polynomial derivative() const;

	template<typename U, typename F>
	friend Polynomial<decltype(U{} + F{}) > operator+(const Polynomial<U>& left, const Polynomial<F>& right);
	template<typename U, typename F>
	friend Polynomial<decltype(U{} - F{}) > operator-(const Polynomial<U>& left, const Polynomial<F>& right);
	template<typename U, typename F>
	friend Polynomial<decltype(U{} *F{}) > operator*(const Polynomial<U>& left, const Polynomial<F>& right);
	template<typename U, typename F>
	friend Polynomial<decltype(U{} / F{}) > operator/(Polynomial<U> left, Polynomial<F> right);
	template<typename U, typename F>
	friend Polynomial<decltype(U{} - U{} / F{}) > operator%(const Polynomial<U>& left, const Polynomial<F>& right);
	template<typename U, typename F>
	friend bool operator==(const Polynomial<U>& left, const Polynomial<F>& right);
	template<typename U, typename F>
	friend bool operator!=(const Polynomial<U>& left, const Polynomial<F>& right);
};

template<typename T>
Polynomial<T>::Polynomial(std::vector<T>& coeff) : coeff{ coeff } { true_degree(); };

template<typename T>
Polynomial<T>::Polynomial(std::size_t degree) :coeff{ std::vector<T>(degree + 1) } {};

template<typename T>
template<typename... Args>
Polynomial<T>::Polynomial(Args... args) : coeff{ static_cast<T>(args)... } { true_degree(); };

template<typename T>
Polynomial<T>::Polynomial(std::initializer_list<T> list) : coeff{ list } { true_degree(); };

template<typename T>
template<typename U>
Polynomial<T>::Polynomial(const Polynomial<U>& other) :coeff{ std::vector<T>(other.coeff.size()) }
{
	for (int i = 0; i < coeff.size(); ++i) coeff[i] = static_cast<T>(other.coeff[i]);
}

template<typename T>
template<typename U>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<U>& other)
{
	coeff = std::vector<T>(other.coeff_().size());
	for (int i = 0; i < coeff.size(); ++i) coeff[i] = static_cast<T>(other.coeff_()[i]);
	return *this;
}

template<typename T>
void Polynomial<T>::true_degree()
{
	while (coeff.size() - 1 && !coeff[coeff.size() - 1])
	{
		coeff.erase(coeff.end() - 1);
	}
}

template<typename T>
std::size_t Polynomial<T>::degree() const { return coeff.size() - 1; };

template<typename T>
const std::vector<T>& Polynomial<T>::coeff_() const { return coeff; };

template<typename T, typename U>
Polynomial<decltype(T{} + U{}) > operator+(const Polynomial<T>& left, const Polynomial<U>& right)
{
	Polynomial<decltype(T{} + U{}) > pol_max, pol_min;
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
	for (i; i <= pol_min.degree(); ++i)
	{
		new_pol.coeff[i] = pol_max.coeff[i] + pol_min.coeff[i];
	}

	for (i; i <= pol_max.degree(); ++i)
	{
		new_pol.coeff[i] = pol_max.coeff[i];
	}

	new_pol.true_degree();
	return new_pol;
}

template<typename T, typename U>
Polynomial<decltype(T{} - U{}) > operator-(const Polynomial<T>& left, const Polynomial<U>& right)
{
	Polynomial<decltype(T{} - U{}) > pol_max, pol_min;
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

	auto new_pol = Polynomial<decltype(T{} - U{}) > (pol_max.degree());
	int i = 0;
	for (i; i <= pol_min.degree(); ++i)
	{
		new_pol.coeff[i] = left.coeff[i] - right.coeff[i];
	}

	for (i; i <= pol_max.degree(); ++i)
	{
		new_pol.coeff[i] = pol_max == left ? pol_max.coeff[i] : -pol_max.coeff[i];
	}

	new_pol.true_degree();
	return new_pol;
}
template<typename T, typename U>
Polynomial<decltype(T{} *U{}) > operator*(const Polynomial<T>& left, const Polynomial<U>& right)
{
	Polynomial<decltype(T{} *U{}) > pol_max, pol_min;
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
	auto new_pol = Polynomial<decltype(T{} *U{}) > (pol_max.degree() + pol_min.degree());
	for (int i = 0; i <= new_pol.degree(); ++i)
	{
		for (int j = 0; j <= i; ++j)
		{
			if (i - j >= pol_max.degree() + 1) continue;
			if (j >= pol_min.degree() + 1) break;
			new_pol.coeff[i] += pol_max.coeff[i - j] * pol_min.coeff[j];
		}
	}

	return new_pol;
}


template<typename T, typename U>
Polynomial<decltype(T{} / U{}) > operator/(Polynomial<T> left, Polynomial<U> right)
{
	if (left.degree() < right.degree()) return Polynomial<decltype(T{} / U{}) > ({ 0 });
	auto new_pol = Polynomial<decltype(T{} / U{}) > (left.degree() - right.degree());

	for (int i = 0; i <= new_pol.degree(); ++i)
	{
		if (left.degree() - i < right.degree()) break;
		if (!left.coeff[left.degree() - i]) continue;
		auto coeff = left.coeff[left.degree() - i] / right.coeff[right.degree()];
		new_pol.coeff[new_pol.degree() - i] = coeff;
		for (int j = 0; j <= right.degree(); ++j)
		{
			left.coeff[left.degree() - right.degree() - i + j] -= right.coeff[j] * coeff;
		}
	}

	return new_pol;
}

template<typename U, typename F>
Polynomial<decltype(U{} - U{} / F{}) > operator%(const Polynomial<U>& left, const Polynomial<F>& right)
{
	return left - left / right;
}


template<typename U, typename F>
bool operator==(const Polynomial<U>& left, const Polynomial<F>& right)
{
	if (left.degree() != right.degree()) return false;
	for (int i = 0; i < left.degree(); ++i)
	{
		if (left.coeff[i] != right.coeff[i]) return false;
	}

	return true;
}

template<typename U, typename F>
bool operator!=(const Polynomial<U>& left, const Polynomial<F>& right)
{
	return !(left == right);
}

template<typename T>
template<typename U>
decltype(T{}*U{}) Polynomial<T>::operator()(U& value) const
{
	decltype(T{}*U{}) result = 0;
	for (int i = 0; i < coeff.size(); ++i) result += coeff[i] * pow(value, i);

	return result;
}

template<typename T>
Polynomial<T> Polynomial<T>::derivative() const
{
	if (!this->degree()) return Polynomial<T>({0});
	auto new_pol = Polynomial<T>(this->degree() - 1);

	for (int i = 0; i <= new_pol.degree(); ++i)
	{
		new_pol.coeff[i] = coeff[i + 1] * (i + 1);
	}

	return new_pol;
}