#pragma once

#include"Polynomial.h"
#include<vector>
#include<numeric>
#include<utility>
#include<limits>

template<typename T>
std::size_t sturm_root_number(Polynomial<T>& pol, double a, double b)
{
	std::vector<Polynomial<T>> sturm_seq;
	sturm_seq.push_back(pol);
	sturm_seq.push_back(pol.derivative());
	while (sturm_seq[sturm_seq.size() - 1].degree())
	{
		sturm_seq.push_back(Polynomial<T>{ { -1} }*(sturm_seq[sturm_seq.size() - 2] % sturm_seq[sturm_seq.size() - 1]));
	}

	int result = 0;
	for (auto x : { a,b })
	{
		bool prev_pos = sturm_seq[0](x) > 0 ? true : false;
		for (int i = 1; i < sturm_seq.size(); ++i)
		{
			auto& pol = sturm_seq[i];
			auto calc = pol(x);
			if ((!prev_pos && calc > 0) || (prev_pos && calc < 0))
			{
				prev_pos = !prev_pos;
				if (x == a)++result;
				else --result;
			}
		}
	}

	return std::abs(result);
}

template<typename T>
std::vector<std::pair<double, double>> rough_sol(Polynomial<T>& pol)
{

	auto coeff = pol.coeff_();
	for (auto& elem : coeff) elem = std::abs(elem);

	auto left_total = coeff[0] / (coeff[0] + *std::max_element(coeff.begin() + 1, coeff.end()));
	auto right_total = 1 + *std::max_element(coeff.begin(), coeff.end() - 1) / coeff[coeff.size() - 1];
	
	return rough_sol(pol, left_total, right_total);
}

template<typename T>
std::vector<std::pair<double, double>> rough_sol(Polynomial<T>& pol, double left_total, double right_total)
{
	auto total = sturm_root_number(pol, left_total, right_total);
	std::vector<std::pair<double, double>> intervals{ total };

	for (int i = 0; i < total; ++i)
	{
		double left, right;
		if (i) left = intervals[i - 1].second + pow(10, -6);
		else left = left_total;
		right = right_total;

		while (true)
		{
			auto middle = (left + right) / 2;
			auto result = sturm_root_number(pol, left, middle);
			if (result > 1)
			{
				right = middle;
			}
			else if (result == 1)
			{
				intervals[i] = std::make_pair(left, middle);
				break;
			}
			else
			{
				left = middle;
			}
		}
	}

	return intervals;
}

template<typename T>
double newton_method(Polynomial<T>& pol, double left_rough, double right_rough, double error=pow(10,-6))
{
	auto der1 = pol.derivative();
	auto der2 = der1.derivative();

	auto der1_roots = rough_sol(der1, left_rough, right_rough);
	auto der2_roots = rough_sol(der2, left_rough, right_rough);

	auto left = left_rough;
	double right;
	bool permission = false;
	for (int i = 0; i < std::min(der1_roots.size(), der2_roots.size()); ++i)
	{
		right = std::min(der1_roots[i].first, der2_roots[i].first);
		if (pol(left)*pol(right) <= 0) { permission = true; break; }
		left = std::max(der1_roots[i].second, der2_roots[i].second);
	}

	if (!permission)
	{
		right = right_rough;
		if (pol(left)*pol(right) <= 0) { permission = true; }
		else throw std::exception("Can't use Newton's method");
	}

	double result, delta = std::numeric_limits<double>::max();
	if (der2(left) > 0) result = right;
	else result = left;

	do
	{	delta = pol(result) / der1(result);
		result -= delta;
	} while (std::abs(delta) > error);

	return result;
}