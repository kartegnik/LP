#include<vector>
#include<iostream>
#include"Simplex.h"


using namespace std;


void reader(int& N, int& M, vector<vector<double>>& a, vector<vector<double>>& b, vector<vector<double>>& c, vector<double> t) {
    int pos = 0;
    N = t[pos++];
    M = t[pos++];
    a.resize(M, vector<double>(N, 0.0));
    b.resize(M, vector<double>(1, 0.0));
    c.resize(N, vector<double>(1, 0.0));
    for (int i = 0; i < N; ++i) c[i][0] = t[pos++];
    for (int i = 0; i < M; ++i) b[i][0] = t[pos++];
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            a[i][j] = t[pos++];
        }
    }
}

void reader2(int& N, int& M, vector<vector<double>>& a, vector<vector<double>>& b, vector<int>& inch, vector<vector<double>>& c, vector<double> t) {
    int pos = 0;
    N = t[pos++];
    M = t[pos++];
    a.resize(M, vector<double>(N, 0));
    b.resize(M, vector<double>(1, 0));
    inch.resize(M, 0);
    c.resize(N, vector<double>(1, 0));
    for (int i = 0; i < N; ++i) c[i][0] = t[pos++];
    for (int i = 0; i < M; ++i) b[i][0] = t[pos++];
    for (int i = 0; i < M; ++i) inch[i] = t[pos++];
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            a[i][j] = t[pos++];
        }
    }
}
/*
    Делаю b >= 0
*/
void setpositiveb(int N, int M, vector<vector<double>>& a, /*vector<int>& inch,*/ vector<vector<double>>& b) {
    for (int i = 0; i < M; ++i) {
        if (Tools::LT(b[i][0],0.0)) {
            for (int j = 0; j < N; ++j)
                a[i][j] = -a[i][j];
            b[i][0] = -b[i][0];
            //inch[i] = -inch[i];
        }
    }
}

vector<vector<double>> setsimplextabluaA(int n, int m, const vector<vector<double>>& a) {
    vector<vector<double>> A(m, vector<double>(n, 0));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n - m; ++j)
            A[i][j] = a[i][j];
    for (int i = 0; i < m; ++i)
        A[i][i + n - m] = 1.0;

    return A;
}

vector<vector<double>> setsimplextabluaA2(int n, int m, const vector<vector<double>>& a, const vector<int>& inch) {
    vector<vector<double>> A(m, vector<double>(n, 0.0));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n - m; ++j)
            A[i][j] = a[i][j];
    for (int i = 0; i < m; ++i)
        if (Tools::GT(inch[i], 0.0))
            A[i][i + n - m] = 1.0;
        else if (inch[i] < 0) {
            A[i][i + n - m] = -1.0;
        }
    return A;
}

vector<vector<double>> setsimplextabluaC(int n, int m, const vector<vector<double>>& c) {
    vector<vector<double>> C(n, vector<double>(1, 0.0));
    for (int i = 0; i < n - m; ++i) C[i][0] = c[i][0];
    return C;
}

vector<bool> setsimplextabluaBASE(int n, int m) {
    vector<bool> base(n, 0);
    for (int i = n - m; i < n; i++) base[i] = 1;
    return base;
}
/*
    Поиск допустимого базисного решения либо задача недопустима
*/
bool fvis(const int n, const int m, vector<vector<double>>& A, vector<vector<double>>& b, vector<bool>& base, double& c0, vector<double>& answer) {
    vector<bool> new_base = setsimplextabluaBASE(n + m, m);
    setpositiveb(n, m, A, b);
    vector<vector<double>> new_c(n + m, vector<double>(1, 0.0));
    for (int i = n; i < n + m; ++i) new_c[i][0] = 1.0;
    vector<double> new_answer;
    vector<vector<double>> new_A = setsimplextabluaA(n + m, m, A);
    simplex0(n + m, m, new_A, b, new_c, new_base, c0, new_answer);
    if (Tools::EQ(c0, 0)) { // теперь надо добиться чтобы базис был в первых n столбцах, пока без пунка 2б из lp2005
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i][j] = new_A[i][j];
            }
        }
        base.resize(n, 0);
        for (int j = 0; j < n; ++j) base[j] = new_base[j];
        answer.resize(n, 0);
        for (int j = 0; j < n; ++j) answer[j] = new_answer[j];
        return true; // и еще надо чето запомнить, чтобы потом в основном алгоритме начать с него как с допустимого решения
    }
    return false;
}

int simplex0(const int n, const int m, vector<vector<double>>& A, vector<vector<double>>& b, vector<vector<double>>& c, 
    vector<bool>& base, double& c0, vector<double>& answer) {
    vector<vector<double>> B(m, vector<double>(m, 0));
    vector<vector<double>> N(m, vector<double>(n - m, 0));
    vector<vector<double>> cN(n - m, vector<double>(1, 0));
    vector<vector<double>> cB(m, vector<double>(1, 0));
    for (int i = 0; i < m; ++i) {
        int count_base = 0;
        for (int j = 0; j < n; ++j) {
            if (base[j]) {
                B[i][count_base] = A[i][j];
                cB[count_base][0] = c[j][0];
                ++count_base;
            }
            else {
                N[i][j - count_base] = A[i][j];
                cN[j - count_base][0] = c[j][0];
            }
        }
    }


    vector<vector<double>> invB = inverse_matrix(B);
    vector<vector<double>> H = mxm(invB, N);
    vector<vector<double>> w = mxm(invB, b);
    vector<vector<double>> yT = mxm(transposition(cB), invB);
    vector<vector<double>> y = transposition(yT);
    c0 = scalar_product(b, y);
    bool wge0 = true;
    for (int i = 0; i < w.size(); ++i) if (Tools::LT(w[i][0], 0.0)) wge0 = false;
    if (wge0) {
        vector<vector<double>> dN = cN;
        int count_base = 0;
        int p = -1;   // наращиваемая переменная
        int idp = 0;
        for (int i = 0; i < n; ++i) {
            if (base[i]) {
                ++count_base;
                continue;
            }
            dN[i - count_base][0] -= scalar_product(transposition(transposition(N)[i - count_base]), y);
            if (Tools::LT(dN[i - count_base][0], 0.0)) {
                if (p == -1) {
                    p = i - count_base;
                    idp = i;
                }
                else if (Tools::LT(dN[i - count_base][0], dN[p][0])) {
                    p = i - count_base;
                    idp = i;
                }
            }
        }
        if (p != -1) {
            int q = -1;   // покидающая базис переменная
            int idq = 0;
            double teta = 1e9;
            int count_base = 0;
            for (int i = 0; i < n; ++i) {
                if (!base[i]) {
                    ++count_base;
                    continue;
                }
                if (Tools::GT(H[i - count_base][p], 0.0)) {
                    if (q == -1) {
                        q = i - count_base;
                        idq = i;
                        teta = w[i - count_base][0] / H[i - count_base][p];
                    }
                    else {
                        if (Tools::LT(w[i - count_base][0] / H[i - count_base][p], teta)) {
                            q = i - count_base;
                            idq = i;
                            teta = w[i - count_base][0] / H[i - count_base][p];
                        }
                    }
                }
            }
            if (q != -1) {
                base[idp] = 1;
                base[idq] = 0;
                return simplex0(n, m, A, b, c, base, c0, answer);
            }
            else {
                return -1; // задача неограничена
            }
        }
        else {
            answer.resize(n, 0);
            int pos = 0;
            for (int i = 0; i < n; ++i) {
                if (base[i])
                    answer[i] = w[pos++][0];
                else
                    answer[i] = 0;
            }
            int count_base = 0;
            for (int k = 0; k < n; ++k) {             // ну вот это немного нечестно, пока не понимаю
                if (base[k]) {
                    for (int i = 0; i < m; ++i) {
                        A[i][k] = (double)(i == count_base);
                    }
                    count_base++;
                }
                else {
                    for (int i = 0; i < m; ++i) {
                        A[i][k] = H[i][k - count_base];
                    }
                }
            }
            b = w;
            return 0; // конец алгоритма
        }

    }
    else {
        return 1; // задача прямо недопустима
    }

}
int simplex1(const int n, const int m, vector<vector<double>>& A, vector<vector<double>>& b, vector<vector<double>>& c,
    vector<bool>& base, double& c0, vector<double>& answer) {
    vector<vector<double>> B(m, vector<double>(m, 0));
    vector<vector<double>> N(m, vector<double>(n - m, 0));
    vector<vector<double>> cN(n - m, vector<double>(1, 0));
    vector<vector<double>> cB(m, vector<double>(1, 0));

    for (int i = 0; i < m; ++i) {
        int count_base = 0;
        for (int j = 0; j < n; ++j) {
            if (base[j]) {
                B[i][count_base] = A[i][j];
                cB[count_base][0] = c[j][0];
                ++count_base;
            }
            else {
                N[i][j - count_base] = A[i][j];
                cN[j - count_base][0] = c[j][0];
            }
        }
    }
    vector<vector<double>> invB = inverse_matrix(B);
    vector<vector<double>> H = mxm(invB, N);
    vector<vector<double>> w = mxm(invB, b);
    vector<vector<double>> yT = mxm(transposition(cB), invB);
    vector<vector<double>> y = transposition(yT);
    c0 = scalar_product(b, y);
    //bool wge0 = true;
    //for (int i = 0; i < w.size(); ++i) if (w[i][0] < 0) wge0 = false;
    //if (wge0) {
        vector<vector<double>> dN = cN;
        int count_base = 0;
        int p = -1;   // наращиваемая переменная
        int idp = 0;
        for (int i = 0; i < n; ++i) {
            if (base[i]) {
                ++count_base;
                continue;
            }
            dN[i - count_base][0] -= scalar_product(transposition(transposition(N)[i - count_base]), y);
            if (dN[i - count_base][0] < 0) {
                if (p == -1) {
                    p = i - count_base;
                    idp = i;
                }
                else if (dN[i - count_base][0] < dN[p][0]) {
                    p = i - count_base;
                    idp = i;
                }
            }
        }
        if (p != -1) {
            int q = -1;   // покидающая базис переменная
            int idq = 0;
            double teta = 1e9;
            int count_base = 0;
            for (int i = 0; i < n; ++i) {
                if (!base[i]) {
                    ++count_base;
                    continue;
                }
                if (H[i - count_base][p] > 0) {
                    if (q == -1) {
                        q = i - count_base;
                        idq = i;
                        teta = w[i - count_base][0] / H[i - count_base][p];
                    }
                    else {
                        if (w[i - count_base][0] / H[i - count_base][p] < teta) {
                            q = i - count_base;
                            idq = i;
                            teta = w[i - count_base][0] / H[i - count_base][p];
                        }
                    }
                }
            }
            if (q != -1) {
                base[idp] = 1;
                base[idq] = 0;
                simplex1(n, m, A, b, c, base, c0, answer);
            }
            else {
                return -1; // задача неограничена
            }
        }
        else {
            answer.resize(n, 0);
            int pos = 0;
            for (int i = 0; i < n; ++i) {
                if (base[i])
                    answer[i] = w[pos++][0];
                else
                    answer[i] = 0;
            }
            int count_base = 0;
            for (int k = 0; k < n; ++k) {             // ну вот это немного нечестно, пока не понимаю
                if (base[k]) {
                    for (int i = 0; i < m; ++i) {
                        A[i][k] = (double)(i == count_base);
                    }
                    count_base++;
                }
                else {
                    for (int i = 0; i < m; ++i) {
                        A[i][k] = H[i][k - count_base];
                    }
                }
            }
            b = w;
            return 0; // конец алгоритма
        }

    //}
    //else {
    //    return 1; // задача прямо недопустима
    //}

}
void print_simplex(const int n, const int m, vector<vector<double>>& A, vector<vector<double>>& b, vector<vector<double>>& c, vector<bool>& base) {
    vector<vector<double>> B(m, vector<double>(m, 0));
    vector<vector<double>> N(m, vector<double>(n - m, 0));
    vector<vector<double>> cN(n - m, vector<double>(1, 0));
    vector<vector<double>> cB(m, vector<double>(1, 0));

    for (int i = 0; i < m; ++i) {
        int count_base = 0;
        for (int j = 0; j < n; ++j) {
            if (base[j]) {
                B[i][count_base] = A[i][j];
                cB[count_base][0] = c[j][0];;
                ++count_base;
            }
            else {
                N[i][j - count_base] = A[i][j];
                cN[j - count_base][0] = c[j][0];
            }
        }
    }
    vector<vector<double>> invB = inverse_matrix(B);
    vector<vector<double>> H = mxm(invB, N);
    vector<vector<double>> w = mxm(invB, b);
    vector<vector<double>> yT = mxm(transposition(cB), invB);
    vector<vector<double>> y = transposition(yT);
    double c0 = scalar_product(b, y);
    bool wge0 = true;
    for (int i = 0; i < w.size(); ++i) if (w[i][0] < 0) wge0 = false;
    if (wge0) {
        //cout << "пошел процесс\n";
        vector<vector<double>> dN = cN;
        int count_base = 0;
        int p = -1;   // наращиваемая переменная
        int idp = 0;
        for (int i = 0; i < n; ++i) {
            if (base[i]) {
                ++count_base;
                continue;
            }
            dN[i - count_base][0] -= scalar_product(transposition(transposition(N)[i - count_base]), y);
            if (dN[i - count_base][0] < 0) {
                if (p == -1) {
                    p = i - count_base;
                    idp = i;
                }
                else if (dN[i - count_base][0] < dN[p][0]) {
                    p = i - count_base;
                    idp = i;
                }
            }
        }
        if (p != -1) {
            int q = -1;   // покидающая базис переменная
            int idq = 0;
            double teta = 1e9;
            int count_base = 0;
            for (int i = 0; i < n; ++i) {
                if (!base[i]) {
                    ++count_base;
                    continue;
                }
                if (H[i - count_base][p] > 0) {
                    if (q == -1) {
                        q = i - count_base;
                        idq = i;
                        teta = w[i - count_base][0] / H[i - count_base][p];
                    }
                    else {
                        if (w[i - count_base][0] / H[i - count_base][p] < teta) {
                            q = i - count_base;
                            idq = i;
                            teta = w[i - count_base][0] / H[i - count_base][p];
                        }
                    }
                }
            }
            if (q != -1) {
                vector<vector<double>> eQ = cr(q, m), eP = cr(p, n - m);
                vector<vector<double>> eQeQT = mxm(eQ, transposition(eQ));
                vector<vector<double>> Hp = transposition(transposition(H)[p]);
                vector<vector<double>> HpeQT = mxm(Hp, transposition(eQ));
                vector<vector<double>> G(m, vector<double>(m, 0)), invG(m, vector<double>(m, 0));
                for (int i = 0; i < m; ++i) G[i][i] = 1.0;
                for (int i = 0; i < m; ++i) {
                    for (int j = 0; j < m; ++j)
                        G[i][j] = G[i][j] - eQeQT[i][j] + HpeQT[i][j];
                }
                base[idp] = 1;
                base[idq] = 0;
                print_simplex(n, m, A, b, c, base);
            }
            else {
                cout << "-INF\n";
            }
        }
        else {
            print(c0);
            print(base);
            int pos = 0;
            for (int i = 0; i < n - m; ++i) {
                if (base[i])
                    cout << "x_" << 1 + i << " = " << w[pos++][0] << "\n";
                else
                    cout << "x_" << 1 + i << " = " << 0 << "\n";

            }
            cout << "Конец алгоритма\n";
        }

    }
    else {
        print("zadacha pryamo nedopustima\n");
    }

}
int simplex2(const int n, const int m, vector<vector<double>>& A, vector<vector<double>>& b, vector<vector<double>>& c,
    vector<bool>& base, double& c0, vector<double>& answer) {
    vector<vector<double>> B(m, vector<double>(m, 0));
    vector<vector<double>> N(m, vector<double>(n - m, 0));
    vector<vector<double>> cN(n - m, vector<double>(1, 0));
    vector<vector<double>> cB(m, vector<double>(1, 0));

    for (int i = 0; i < m; ++i) {
        int count_base = 0;
        for (int j = 0; j < n; ++j) {
            if (base[j]) {
                B[i][count_base++] = A[i][j];
            }
            else {
                N[i][j - count_base] = A[i][j];
            }
        }
    }
    int count_base = 0;
    for (int j = 0; j < n; ++j) {
        if (base[j])
            cB[count_base++][0] = c[j][0];
        else
            cN[j - count_base][0] = c[j][0];
    }
    bool presol = fvis(n, m, A, b, base, c0, answer);
    vector<vector<double>> invB = inverse_matrix(B);
    vector<vector<double>> H = mxm(invB, N);
    vector<vector<double>> HT = transposition(H);
    vector<vector<double>> w = mxm(invB, b);
    vector<vector<double>> yT = mxm(transposition(cB), invB);
    vector<vector<double>> y = transposition(yT);
    do {
        double c0 = scalar_product(b, y);
        bool wge0 = true;
        for (int i = 0; i < w.size(); ++i) if (Tools::LT(w[i][0], 0.0)) wge0 = false;
        if (wge0) {
            cout << "пошел процесс\n";
            vector<vector<double>> dN = cN;
            int count_base = 0;
            int p = -1;   // наращиваемая переменная
            int idp = 0;
            for (int i = 0; i < n; ++i) {
                if (base[i]) {
                    ++count_base;
                    continue;
                }
                dN[i - count_base][0] -= scalar_product(transposition(transposition(N)[i - count_base]), y);
                if (Tools::LT(dN[i - count_base][0], 0.0)) {
                    if (p == -1) {
                        p = i - count_base;
                        idp = i;
                    }
                    else if (Tools::LT(dN[i - count_base][0], dN[p][0])) {
                        p = i - count_base;
                        idp = i;
                    }
                }
            }
            if (p != -1) {
                int q = -1;   // покидающая базис переменная
                int idq = 0;
                double teta = 1e9;
                int count_base = 0;
                for (int i = 0; i < n; ++i) {
                    if (!base[i]) {
                        ++count_base;
                        continue;
                    }
                    if (Tools::GT(H[i - count_base][p], 0.0)) {
                        if (q == -1) {
                            q = i - count_base;
                            idq = i;
                            teta = w[i - count_base][0] / H[i - count_base][p];
                        }
                        else {
                            if (Tools::LT(w[i - count_base][0] / H[i - count_base][p], teta)) { // а тут можно w < teta*H
                                q = i - count_base;
                                idq = i;
                                teta = w[i - count_base][0] / H[i - count_base][p];
                            }
                        }
                    }
                }
                if (q != -1) {
                    vector<vector<double>> eQ = cr(q, m), eP = cr(p, n - m);
                    vector<vector<double>> eQeQT = mxm(eQ, transposition(eQ));
                    vector<vector<double>> Hp = transposition(HT[p]);
                    vector<vector<double>> HpeQT = mxm(Hp, transposition(eQ));
                    vector<vector<double>> G(m, vector<double>(m, 0)), invG(m, vector<double>(m, 0));
                    for (int i = 0; i < m; ++i) G[i][i] = 1.0;
                    for (int i = 0; i < m; ++i) {
                        for (int j = 0; j < m; ++j)
                            G[i][j] = G[i][j] - eQeQT[i][j] + HpeQT[i][j];
                    }
                    base[idp] = 1;
                    base[idq] = 0;
                    //B = mxm(B, G);
                    invG = inverse_matrix(G);
                    invB = mxm(invG, invB);
                    H = mxm(invG, H);
                    HT = transposition(H);
                    w = mxm(invG, w);
                    yT = mxm(yT, invG);
                    y = transposition(yT);
                    for (int i = 0; i < m; ++i) {
                        int count_base = 0;
                        for (int j = 0; j < n; ++j) {
                            if (base[j]) {
                                B[i][count_base++] = A[i][j];
                            }
                            else {
                                N[i][j - count_base] = A[i][j];
                            }
                        }
                    }
                    int count_base = 0;
                    for (int j = 0; j < n; ++j) {
                        if (base[j])
                            cB[count_base++][0] = c[j][0];
                        else
                            cN[j - count_base][0] = c[j][0];
                    }
                }
                else {
                    return -1;
                }
            }
            else {
                return 0;
            }

        }
        else {
            return 1;
            break;
        }
    } while (true);

}
