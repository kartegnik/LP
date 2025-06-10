#pragma once
#include "tools.h"

int findPivot(const std::vector<std::vector<double>>& a, int x);
void swapString(std::vector<std::vector<double>>& a, int x, int y);
void subs(std::vector<std::vector<double>>& a, int x, int y);
double detgauss(std::vector<std::vector<double>> a);
double minor(const std::vector<std::vector<double>>& a, int x, int y);
double adj(const std::vector<std::vector<double>>& a, int x, int y);
std::vector<std::vector<double>> transposition(const std::vector<std::vector<double>>& a);
std::vector<std::vector<double>> transposition(const std::vector<double>& a);
std::vector<std::vector<double>> inverse_matrix(const std::vector<std::vector<double>>& a);
std::vector<std::vector<double>> mxm(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b);
std::vector<std::vector<double>> mxm(const std::vector<double>& a, const std::vector<std::vector<double>>& b);
std::vector<std::vector<double>> mxm(const std::vector<std::vector<double>>& a, const std::vector<double>& b);
std::vector<std::vector<double>> cr(int x, int n);
double scalar_product(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b);
double scalar_product(const std::vector<double>& a, const std::vector<std::vector<double>>& b);
double scalar_product(const std::vector<std::vector<double>>& a, const std::vector<double>& b);
double scalar_product(const std::vector<double> a, const std::vector<double>& b);
void print(const std::vector<std::vector<double>>& a);
void print(const std::vector<double>& a);
void print(const std::vector<int>& a);
void print(const std::vector<bool>& a);
void print(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b);
void print(const double& a);
void print(std::string s);