#include<vector>
#include"Gomori.h"

using namespace std;

double properFraction(double x) {
    // Если почти целое — дробная часть = 0
    if (Tools::EQ(x, std::round(x))) {
        return 0.0;
    }
    double fl = std::floor(x);
    double f;
    f = x - fl;
    // «почти» 1 тоже считаем 0
    if (Tools::EQ(f, 1.0)) {
        return 0.0;
    }
    return f;
}

// ======= Функция поиска максимальной дробной части =======
double maxProperFraction(const vector<vector<double>>& v) {
    // инициализируем максимум дробной частью первого элемента
    double f0 = properFraction(v[0][0]);
    double maxF = f0;
    int   bestIdx = 0;
    bool  anyFrac = Tools::NE(f0, 0.0);  // есть ли хоть одна «не-нулевая» дробь

    for (size_t i = 1; i < v.size(); ++i) {
        double f = properFraction(v[i][0]);
        if (Tools::NE(f, 0.0)) {
            anyFrac = true;
        }
        if (Tools::GT(f, maxF)) {
            maxF = f;
            bestIdx = int(i);
        }
    }

    // если не нашли ни одной ненулевой дроби
    if (!anyFrac) {
        return -1;
    }
    return bestIdx;
}
// еще надо добавить если слева все коэфы целый а справа дробное число то недопустимая задача
int gomori(int n, int m, vector<vector<double>>& A, vector<vector<double>>& b, vector<vector<double>>& c, vector<bool>& base, double& c0, vector<double>& answer) {
    bool presol = fvis(n, m, A, b, base, c0, answer);
    //print(A, b);
    if (presol) {
        int simplx = simplex0(n, m, A, b, c, base, c0, answer);
        //print(A, b);
        //print(answer);
        if (simplx == -1) {
            return -1;
        }
        else {
            int all_integer = maxProperFraction(b);
            if (all_integer == -1) {
                return 0;
            }
            else {
                b.push_back(vector{ -properFraction(b[all_integer][0]) });
                base.push_back(1);
                c.push_back(vector{ 0.0 });
                vector<double> new_row(n + 1, 0);
                for (int j = 0; j < n; ++j) {
                    new_row[j] = -properFraction(A[all_integer][j]);
                }
                new_row.back() = 1.0;
                for (int i = 0; i < m; ++i) {
                    A[i].push_back(0.0);
                }
                A.push_back(new_row);
                n++;
                m++;
                //print(A, b);
                return gomori(n, m, A, b, c, base, c0, answer);
            }
        }
    }
    else
        return 1; // задача прямо недопустима
}