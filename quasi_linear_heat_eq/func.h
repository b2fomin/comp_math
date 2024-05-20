#pragma once
#include<cmath>

double k(double t, double x, double y, double u)
{
	return pow(u, 1. / 3);
}

double u_left(double t, double y)
{
	return pow((1 + y), 6) / pow(30-28*t, 3);
}

double u_right(double t, double y)
{
	return pow(2 + y, 6) / pow(30 - 28 * t, 3);
}

double u_top(double t, double x)
{
	return pow(2 + x, 6) / pow(30 - 28 * t, 3);
}

double u_bottom(double t, double x)
{
	return pow(1 + x, 6) / pow(30 - 28 * t, 3);
}

double u_init(double x, double y)
{
	return pow(1 + x + y, 6) / 27000;
}