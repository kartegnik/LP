#include<vector>
#include<string>
#include<iostream>
#include"inverse_matrix.h"

using namespace std;


int findPivot(const vector<vector<double>>& a, int x) {
    for (int i = x; i < a.size(); ++i) {
        if (Tools::NE(a[i][x],0.0))
            return i;
    }
    return -1;
}

void swapString(vector<vector<double>>& a, int x, int y) {
    for (int i = 0; i < a[0].size(); ++i) {
        swap(a[x][i], a[y][i]);
    }
}


void subs(vector<vector<double>>& a, int x, int y) { // x - строка которой вычитают, y - строка из которой вычитают
    double c = a[y][x] / a[x][x];
    if (c == 0) return;
    for (int i = x; i < a[0].size(); ++i) {
        a[y][i] -= a[x][i] * c;
    }
}


double detgauss(vector<vector<double>> a) {
    int sign = 0;
    for (int i = 0; i < a.size() - 1; ++i) {
        int Pivot = findPivot(a, i);
        if (Pivot == -1) continue;
        if (Pivot != i) {
            swapString(a, Pivot, i);
            ++sign;
        }
        for (int j = i + 1; j < a.size(); ++j) {
            subs(a, i, j);
        }
    }
    double det = (sign % 2 == 0 ? 1 : -1);
    for (int i = 0; i < a.size(); ++i) {
        det *= a[i][i];
    }
    return det;
}

double minor(const vector<vector<double>>& a, int x, int y) {
    vector<vector<double>> m(a.size() - 1, vector<double>(a.size() - 1, 0.0));
    for (int i = 0; i < x; ++i) {
        int yc = 0;
        for (int j = 0; j < a.size(); ++j) {
            if (j == y) { yc = 1; continue; }
            m[i][j-yc] = a[i][j];
        }
    }
    for (int i = x + 1; i < a.size(); ++i) {
        int yc = 0;
        for (int j = 0; j < a.size(); ++j) {
            if (j == y) { yc = 1; continue; }
            m[i - 1][j-yc] = a[i][j];
        }
    }
    return detgauss(m);
}

double adj(const vector<vector<double>>& a, int x, int y) {
    if ((x + y) % 2 == 0) return minor(a, x, y);
    return -minor(a, x, y);
}

vector<vector<double>> transposition(const vector<vector<double>>& a) {
    int n = a.size();
    int m = a[0].size();
    vector<vector<double>> tr(m, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            tr[j][i] = a[i][j];
        }
    }
    return tr;
}

vector<vector<double>> transposition(const vector<double>& a) {
    vector<vector<double>> tmp(1, vector<double>(a.size(), 0.0));
    for (int i = 0; i < a.size(); ++i) tmp[0][i] = a[i];
    return transposition(tmp);
}

vector<vector<double>> inverse_matrix(const vector<vector<double>>& a) {
    int n = a.size();
    int m = a[0].size();
    if (n == 1 && m == 1) return { {1 / a[0][0]} };
    double det = detgauss(a);
    vector<vector<double>> inv(m, vector<double>(n, 0.0));
    vector<vector<double>> tr = transposition(a);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            inv[i][j] = adj(tr, i, j) / det;
        }
    }
    return inv;
}

vector<vector<double>> mxm(const vector<vector<double>>& a, const vector<vector<double>>& b) {
    vector<vector<double>> c(a.size(), vector<double>(b[0].size(), 0.0));
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b[0].size(); ++j) {
            for (int k = 0; k < b.size(); ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}


vector<vector<double>> mxm(const vector<double>& a, const vector<vector<double>>& b) {
    vector<vector<double>> tmp(1, vector<double>(a.size(), 0.0));
    for (int i = 0; i < a.size(); ++i) tmp[0][i] = a[i];
    return mxm(tmp, b);
}

vector<vector<double>> mxm(const vector<vector<double>>& a, const vector<double>& b) {
    vector<vector<double>> tmp(1, vector<double>(b.size(), 0.0));
    for (int i = 0; i < b.size(); ++i) tmp[0][i] = b[i];
    return mxm(a, tmp);
}

vector<vector<double>> cr(int x, int n) {
    vector<vector<double>> eX(n, vector<double>(1, 0.0));
    eX[x][0] = 1.0;
    return eX;
}

double scalar_product(const vector<vector<double>>& a, const vector<vector<double>>& b) {
    return mxm(transposition(a), b)[0][0];
}

double scalar_product(const vector<double>& a, const vector<vector<double>>& b) {
    return mxm(transposition(a), b)[0][0];
}

double scalar_product(const vector<vector<double>>& a, const vector<double>& b) {
    return scalar_product(b, a);
}

double scalar_product(const vector<double> a, const vector<double>& b) {
    return mxm(transposition(a), b)[0][0];
}

void print(const vector<vector<double>>& a) {
    for (auto i : a) {
        for (double j : i) cout << j << "\t";
        cout << endl;
    }
    cout << endl;
}

void print(const vector<vector<double>>& a, const vector<vector<double>>& b) {
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < a[0].size(); ++j) cout << a[i][j] << "\t";
        cout << b[i][0] << endl;
    }
    cout << endl;
}

void print(const vector<double>& a) {
    for (double i : a) cout << i << "\t";
    cout << endl;
}

void print(const vector<int>& a) {
    for (int i : a) cout << i << "\t";
    cout << endl;
}

void print(const vector<bool>& a) {
    for (bool i : a) cout << i << "\t";
    cout << endl;
}

void print(const double& a) {
    cout << a << endl;
}

void print(string s) {
    cout << s << endl;
}