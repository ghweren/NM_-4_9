#include <iostream>
#include "NewtonsPolinomial.h"
#include <time.h>
#include <list>
#include <algorithm>
#include <utility>
using namespace std;

bool compare(const pair<int, double> &left, const pair<int, double> &right)
{
	return left.second < right.second;
}

vector<int> search_nearest_points(vector<double> x,double XX)
{
	vector<pair<int, double>> x_pair;
	for (int i = 0; i < x.size(); i++)
		x_pair.push_back(make_pair(i, abs(x[i] - XX)));

	sort(begin(x_pair), end(x_pair), compare);
	vector<int> result;
	for (int i = 0; i < x.size(); i++)
		result.push_back(x_pair[i].first);
	return result;
}
vector<double> create_x_temp(vector<double> x, vector<int> indexes, int k)
{
	list<double> temp;
	vector<double> result;
	double prev = x[indexes[0]];
	temp.push_back(prev);
	for (int i = 1; i < k;i++)
	if (x[indexes[i]] < prev)
	{
		temp.push_front(x[indexes[i]]);
		prev = x[indexes[i]];
	}
	else
	{
		temp.push_back(x[indexes[i]]);
		prev = x[indexes[i]];
	}
	for (auto it = temp.begin(); it != temp.end(); it++)
		result.push_back(*it);
	return result;
}

vector<double> create_y_temp(vector<double> x, vector<double>y, vector<int> indexes, int k)
{
	list<double> temp;
	vector<double> result;
	double prev = x[indexes[0]];
	temp.push_back(y[indexes[0]]);
	for (int i = 1; i < k; i++)
	if (x[indexes[i]] < prev)
	{
		temp.push_front(y[indexes[i]]);
		prev = x[indexes[i]];
	}
	else
	{
		temp.push_back(y[indexes[i]]);
		prev = x[indexes[i]];
	}
	for (auto it = temp.begin(); it != temp.end(); it++)
		result.push_back(*it);
	return result;
}
double function(double x)
{
	return x*x*x;
}
vector <double> x_generation(int N)
{
	vector<double> x;
	srand(time(NULL));
	x.push_back(rand() % 10);
	for (int i = 1; i < N; i++)
		x.push_back(rand() % 10 + 1 + x[i - 1]);
	return x;
}

vector<double> y_generation(vector<double> x,double(*function)(double))
{
	vector<double> y;
	for (int i = 0; i<x.size(); i++)
		y.push_back(function(x[i]));
	return y;
}
int main()
{
	int N;
	double eps, XX;
	srand(time(NULL));
	InterpolationMethod *interpolationMethod = new NewtonsPolinomial;
	try{
		cout << "Enter the N: ";
		cin >> N;
		if (N < 2)
			throw invalid_argument("Not enough points");
		cout << "\nEnter the eps: ";
		cin >> eps;
		if (eps < 0)
			throw invalid_argument("The eps is less than zero");
		vector<double> x = x_generation(N);
		vector<double> y = y_generation(x, function);
		cout << '\n';
		for (int i = 0; i < N; i++)
			cout << x[i] << ' ';
		cout << '\n';
		for (int i = 0; i < N; i++)
			cout << y[i] << ' ';
		cout << '\n';
		for (int i = 1; i < x.size(); i++)
		if (x[i - 1] >= x[i])
			throw invalid_argument("The order of increasing arguments is broken in vector X");
		cout << "\nEnter the XX: ";
		cin >> XX;
		if (XX<x[0] || XX>x[N - 1])
			throw invalid_argument("The value of the argument XX does not belong to the segment [X0,Xn]");
		double Y_real = function(XX);
		vector<int> indexes = search_nearest_points(x, XX);

		vector<double> temp_x=create_x_temp(x,indexes,2);

		vector<double> temp_y=create_y_temp(x,y,indexes,2);

		double prev_result = interpolationMethod->Calculation(temp_y, temp_x, XX);

		if (x.size() == 2)
			cout << "\nThe required accuracy has not been achieved (N = 2)\nThe result of the interpolation: " << prev_result;
		else if (x.size() == 3)
		{
			temp_x=create_x_temp(x, indexes, 3);
			temp_y = create_y_temp(x, y, indexes, 3);
			double result = interpolationMethod->Calculation(temp_y, temp_x, XX);
			if (abs(prev_result-result)<eps)
				cout << "\nThe result of the interpolation: " << result;
			else 
				cout << "\nThe required accuracy has not been achieved (N is too small)\nThe result of the interpolation: " << result;
		}
		else
		{
			temp_x = create_x_temp(x, indexes, 3);
			temp_y = create_y_temp(x, y, indexes, 3);
			double result = interpolationMethod->Calculation(temp_y, temp_x, XX);
			double prev_eps = abs(prev_result - result);
			prev_result = result;
			temp_x = create_x_temp(x, indexes, 4);
			temp_y = create_y_temp(x, y, indexes, 4);
			result = interpolationMethod->Calculation(temp_y, temp_x, XX);
			double curr_eps = abs(prev_result - result);
			
			int i = 4;
			while (i < N&&prev_eps != curr_eps&&curr_eps < eps)
			{
				cout<<1;
				prev_result = result;
				temp_x = create_x_temp(x, indexes, i+1);
				temp_y = create_y_temp(x, y, indexes, i+1);
				result = interpolationMethod->Calculation(temp_y, temp_x, XX);
				prev_eps = curr_eps;
				curr_eps = abs(prev_result - result);
				i++;
			}
			if (curr_eps<eps)
				cout << "\nThe required accuracy has been achieved\nThe result of the interpolation: " << result;
			else if (prev_eps==curr_eps)
				cout << "\nThe modulus of the difference between two consecutive interpolation values has stopped decreasing\nThe result of the interpolation: " << result;
			else
				cout << "\nThe required accuracy has not been achieved (N is too small)\nThe result of the interpolation: " << result;
		}
	}
	catch (exception e)
	{
		cout << e.what();
	}
	cin >> XX;
}