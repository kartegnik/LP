#pragma once
#include"Simplex.h"

double properFraction(double x);
double maxProperFraction(const std::vector<std::vector<double>>& v);
int gomori(int n, int m, std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& b, std::vector<std::vector<double>>& c, std::vector<bool>& base, double& c0, std::vector<double>& answer);