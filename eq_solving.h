#pragma once

#include"Polynomial.h"
#include<vector>
#include<numeric>

template<typename T>
int Sturm_root_number(Polynomial<T>& pol, double a, double b)
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

	return result;
}