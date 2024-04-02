#pragma once
#include "InterpolationMethod.h"
using namespace std;
class NewtonsPolinomial :public InterpolationMethod
{
public:
	double Calculation(vector <double> f, vector <double> x, double t)
	{
		vector<double> div_diff;
		for (int i = 1; i < x.size(); i++)
			div_diff.push_back(divided_difference(f, x, i));
		double result = f[0];
		for (int i = 1; i < x.size(); i++)
		{
			double mul = 1;
			for (int j = 0; j < i; j++)
				mul *= (t - x[j]);

			result += div_diff[i - 1] * mul;
		}
		return result;
	}

	double divided_difference(vector <double> f, vector <double> x, int k)
	{
		double result = 0;
		for (int i = 0; i <= k; i++)
		{
			double mul = 1;
			for (int j = 0; j <= k; j++)
			if (i != j)
				mul *= (x[i] - x[j]);
			result += f[i] / mul;
		}
		return result;
	}
};