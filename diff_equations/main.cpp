#include<iostream>
#include<vector>
#include<limits>
#include<locale>
#include<iomanip>
#include"diff_eq_solving.h"

double norm(const std::vector<double>& y1, const std::vector<double>& y2)
{
    double result = std::numeric_limits<double>::min();
    auto max_size = std::max(y1.size(), y2.size());
    auto min_size = std::min(y1.size(), y2.size());
    auto& y_max = y1.size() == max_size ? y1 : y2;
    auto& y_min = y1.size() == min_size ? y1 : y2;
    double ratio = static_cast<double>(min_size) / max_size;
    for (int i = 0; i < max_size; ++i)
    {
        result = std::max(result, std::abs(
            y_max[i] - y_min[static_cast<std::size_t>(i * ratio)]));
    }
    return result;
}

double f(double x, double y)
{
    return -(3 * y * y + 2 * x * y + 2 * x) /
        (6 * x * y + x * x + 3);
}

int main()
{
    std::setlocale(LC_ALL, "en_US.UTF-8");
    std::cout << "Концы интервала через пробел: ";
    double x_0, x_n;
    std::cin >> x_0 >> x_n;
    std::cout << std::endl << "Точность: ";
    double eps; std::cin >> eps;
    std::cout << std::endl << "Начальные условия: ";
    double y_init; std::cin >> y_init;
    std::cout << std::endl << "Количество выводимых точек: ";
    std::size_t count; std::cin >> count;
    std::cout << std::endl;
    auto& Butcher_table = Heun_table;

    double h = x_n - x_0;
    std::vector<double> y1, y2;
    do
    {
        h /= 2;
        y1 = Runge_Kutta(f, x_0, x_n, (x_n - x_0) / h, y_init, Butcher_table);
        y2 = Runge_Kutta(f, x_0, x_n, (x_n - x_0) / (2 * h), y_init, Butcher_table);
    } while (norm(y1, y2) / (pow(2, Butcher_table.size() - 1) - 1) > eps);

    std::cout.setf(std::ios::scientific);
    std::cout << std::setprecision(5);

    std::cout << "Шаг h: " << h << std::endl;

    double ratio = static_cast<double>(y1.size()) / count;
    for (int i = 0; i < count; ++i)
    {
        auto j = static_cast<std::size_t>(i * ratio);
        auto x = x_0 + j * h;
        std::cout << x << '\t' << y1[j] << std::endl;
    }
}
