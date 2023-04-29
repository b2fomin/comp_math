#pragma once
#include<vector>
#include<limits>
#include<iomanip>
#include<cmath>
#include<fstream>
#include<future>
#include<thread>
#include<mutex>
#include"func.h"

int main()
{
	constexpr double eps = 8e-3;
	constexpr int ratio = 2;
	std::size_t L = 2;
	std::vector<double> u;
	bool stop = false;
	double norm_x, norm_t, prev_norm_x, prev_norm_t;
	norm_x = norm_t = std::numeric_limits<double>::max();

	while (!stop)
	{
		prev_norm_x = norm_x; norm_x = 0;
		prev_norm_t = norm_t; norm_t = 0;
		stop = true;

		u = std::vector<double>(L, 0);
		std::vector<double> vec_norm_x(1), vec_norm_t(L, 0);
		double h = 1. / (L - 1), t = 0;
		for (int i = 0; i < L; ++i)
		{
			u[i] = varphi(i * h);
			vec_norm_x[0] += u[i] * u[i];
			vec_norm_t[i] += u[i] * u[i];
		}

		while(t < 1)
		{
			auto a_max = *std::max_element(u.begin(), u.end());
			auto tau = 2 * h / a_max;
			t += tau;
			vec_norm_x.push_back(0);
			auto u_copy = std::vector<double>(u);

			const std::size_t max_threads = u_copy.size();
			const std::size_t hardware_threads = std::thread::hardware_concurrency();
			const std::size_t num_threads = std::min(hardware_threads == 0 ? 2 : hardware_threads, max_threads);
			std::vector<std::future<void>> futures(num_threads - 1);
			auto lambda = [&u_copy, &u, &vec_norm_x, &vec_norm_t, h, tau, t](std::size_t first, std::size_t last)
			{
				static std::mutex mutex;
				auto x_index = vec_norm_x.size() - 1;
				for (int j = first; j < last; ++j)
				{
					auto x = j * h;
					auto a_ij = a(x, t);
					std::lock_guard<std::mutex> lock(mutex);
					u[j] = u_copy[j] + tau * tau * a_ij * a_ij * (u_copy[j + 2] - 2 * u_copy[j + 1] + u_copy[j]) / (2 * h * h) -
						tau * (a_ij + tau / 2 * (a_t(x, t, tau) - a_ij * a_x(x, t, h))) * (4 * u_copy[j + 1] - 3 * u_copy[j] - u_copy[j + 2]) / (2 * h) +
						tau * b(x, t) + tau * tau / 2 * (b_t(x, t, tau) - a_ij * b_x(x, t, h));
					vec_norm_t[j] += u[j] * u[j];
					vec_norm_x[x_index] += u[j] * u[j];
				}
			};
			auto threads_ratio = static_cast<double>(L - 2) / num_threads;
			for (int i = 0; i < futures.size(); ++i) 
				futures[i] = std::async(lambda, i * threads_ratio, (i + 1) * threads_ratio);

			lambda((num_threads - 1) * threads_ratio, L - 2);
			for (auto& future : futures) future.get();

			u[L - 1] = psi(t);
			vec_norm_t[L - 1] += u[L - 1] * u[L - 1];

			u[L - 2] = u[L - 1] - h / a(1, t) * (b(1, t) - psi_1st(t, tau)) +
				h * h / (2 * a(1, t) * a(1, t)) * (psi_2nd(t, tau) + (b(1, t) - psi_1st(t, tau)) / a(1, t) *
					(a_t(1, t, tau) - a(1, t) * a_x(1, t, h)) - b_t(1, t, tau) + a(1, t) * b_x(1, t, h));
			vec_norm_t[L - 2] += u[L - 2] * u[L - 2];

			vec_norm_x[vec_norm_x.size() - 1] += u[L - 1] * u[L - 1] + u[L - 2] * u[L - 2];
		}

		auto N = vec_norm_x.size();
		for (int i = 0; i < vec_norm_x.size(); ++i) norm_x += std::sqrt(vec_norm_x[i]);
		norm_x /= N*L;

		for (int i = 0; i < vec_norm_t.size(); ++i) norm_t += std::sqrt(vec_norm_t[i]);
		norm_t /= N*L;

		if (std::abs(prev_norm_x - norm_x) > eps) { L *= ratio; stop = false; }

		if (std::abs(prev_norm_t - norm_t) > eps) { N *= ratio; stop = false; }
	}

	double h = 1. / (u.size() - 1);
	std::fstream file("./output.txt", std::ios_base::out);
	file << std::setprecision(6);
	for (int i = 0; i < u.size(); ++i)
		file << h * i << '\t' << u[i] << std::endl;
	file.close();
}