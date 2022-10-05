#pragma once
#include<vector>

template<typename T>
class Polynomial
{
private:
	std::vector<T> coeff;
public:
	Polynomial() = default;
	Polynomial(std::vector<T>& coeff);
	template<typename... Args>
	Polynomial(Args... args);
};

template<typename T>
Polynomial<T>::Polynomial(std::vector<T>& coeff) : coeff{ coeff } {};

template<typename T>
template<typename... Args>
Polynomial<T>::Polynomial(Args... args)
{

}
