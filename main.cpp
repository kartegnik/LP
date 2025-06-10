#include<vector>
#include<iostream>
#include <iomanip>
#include"Simplex.h"
#include"tests.h"

using namespace std;



void solve() {
    Tools::setEps(1e-6);
    int N, M;
    vector<vector<double>> a, b, c;
    vector<int> basis_indices;
    for (int k = 6; k < 7 ; ++k) {
        vector<double> tmp_test = testGOMORI[k];
        vector<int> sense;
        reader2(N, M, a, b, sense, c, tmp_test);
        int n = N + M;
        int m = M;
        vector<vector<double>> A = setsimplextabluaA(n, m, a);  // получили каноническую злп
        vector<bool> base;
        double c0;
        vector<double> answer;
        print(A, b);
        bool presol = true;
        if (presol) {
            cout << k << endl;
            vector<vector<double>> C = setsimplextabluaC(n, m, c);
            //int res = simplex0(n, m, A, b, C, base, c0, answer);
            int res = 0;
            if (res == -1) cout << "-INF\n";
            else if (res == 1) cout << "+ne dopustima\n";
            else {
                int g = simplex2(n, m, A, b, C, base, c0, answer);
                print(g);
                if (g == 0) {
                    print(answer);
                    print(Tools::EQ(c0, tmp_test.back()));
                    print(c0);
                    print(tmp_test.back());
                    print(A, b);
                    print(c);
                    cout << endl;
                }
                else if (g == 1) cout << "+ne dopustima\n";
                else cout << "-INF\n";
            }
        }
        else cout << "ne norm\n";
    }
}


int main() {
    setlocale(LC_ALL, "");
    ios_base::sync_with_stdio(false); cin.tie(0);
    int t = 1;
    //cin >> t;
    cout << fixed << setprecision(2);
    while (t--) {
        solve();
    }
    return 0;
}
