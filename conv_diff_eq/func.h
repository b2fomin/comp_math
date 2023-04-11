#pragma once
#include<cmath>

double a(double x, double t)
{
	return -(x + std::exp(-t));
}

double b(double x, double t)
{
	return x * (x + std::exp(-t));
}

double varphi(double x)
{
	return -x * x / 2 + x;
}

double psi(double t)
{
	return std::exp(t) + t - .5;
}
template<typename T>
auto der_1st(T f, double dx)
{
	return [f, dx](double x)
	{return (f(x + dx) - f(x - dx)) / (2 * dx); };
}

template<typename T>
auto der_2nd(T f, double dx)
{
	return [f, dx](double x)
	{return (f(x + dx) + f(x - dx) - 2 * f(x)) / (2 * dx * dx); };
}

auto a_t(double x, double t, double dt)
{
	return der_1st([x, dt](double t) {return a(x, t); }, dt)(t);
}
auto a_x(double x, double t, double dx)
{
	return der_1st([t, dx](double x) {return a(x, t); }, dx)(x);
}

auto b_t(double x, double t, double dt)
{
	return der_1st([x, dt](double t) {return b(x, t); }, dt)(t);
}
auto b_x(double x, double t, double dx)
{
	return der_1st([t, dx](double x) {return b(x, t); }, dx)(x);
}

auto psi_1st(double t, double dt)
{
	return der_1st(psi, dt)(t);
}
auto psi_2nd(double t, double dt)
{
	return der_2nd(psi, dt)(t);
}
