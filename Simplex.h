#pragma once
#include"inverse_matrix.h"

void reader(int& N, int& M, std::vector<std::vector<double>>& a, std::vector<std::vector<double>>& b, std::vector<std::vector<double>>& c, std::vector<double> t);
void reader2(int& N, int& M, std::vector<std::vector<double>>& a, std::vector<std::vector<double>>& b, std::vector<int>& inch, std::vector<std::vector<double>>& c, std::vector<double> t);
/*
    Ну это надо как-то сразу наверное будет в структуре делать
*/
void setpositiveb(int N, int M, std::vector<std::vector<double>>& a,/* std::vector<int>& inch,*/ std::vector<std::vector<double>>& b);
std::vector<std::vector<double>> setsimplextabluaA(int n, int m, const std::vector<std::vector<double>>& a);
std::vector<std::vector<double>> setsimplextabluaA2(int n, int m, const std::vector<std::vector<double>>& a, const std::vector<int>& inch);
std::vector<std::vector<double>> setsimplextabluaC(int n, int m, const std::vector<std::vector<double>>&c);
std::vector<bool> setsimplextabluaBASE(int n, int m);
void print_simplex(const int n, const int m, std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& b, std::vector<std::vector<double>>& c, std::vector<bool>& base);
/*
    Допустима ли задача (finding a valid initial solution)
*/
bool fvis(const int n, const int m, std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& b,
    std::vector<bool>& base, double& c0, std::vector<double>& answer);
/*
    Симплекс метод, только на МИНИМУМ
    -1 - задача неограничена снизу (двойственно недопустима)
    0  - все успешно посчиталось
    1  - задача неограничена сверху(прямо недопустима)
    n  - количество переменных вместе с дополнительными
    m  - количесво строк
    А  - конечная симплкс таблица
    answer - значения базисных и небазисных переменных(если 0 то небазисная)
    c0 - минимум ЦФ
*/
int simplex0(const int n, const int m, std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& b, std::vector<std::vector<double>>& c,
    std::vector<bool>& base, double& c0, std::vector<double>& answer);
int simplex1(const int n, const int m, std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& b, std::vector<std::vector<double>>& c,
    std::vector<bool>& base, double& c0, std::vector<double>& answer);
int simplex2(const int n, const int m, std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& b, std::vector<std::vector<double>>& c,
    std::vector<bool>& base, double& c0, std::vector<double>& answer);