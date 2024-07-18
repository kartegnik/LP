#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"
#include <typeinfo> 


using namespace alglib;

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Rus");
    setlocale(LC_NUMERIC, "en_US");
    try
    {
        /*
            f(x1,x2) = 5x1 + 6x2 -> max
            3x1 + 5x2 ≤ 55
            x1 + 5x2 ≤ 45
            5x1 + 2x2 ≤ 60
            70x1 + 60x2 ≤ 1500
        */
        int n, m, NNZ;
        sparsematrix a;
        real_1d_array bndl, bndu, al, au, x, c;
        minlpstate state;
        minlpreport rep;
        FILE* f = fopen("testlp3_rf.txt", "r");

        fscanf(f, "%d %d", &n, &m);
        sparsecreate(m, n, a);
        char* xtype = new char[n];
        double* _bndl = new double[n];
        double* _bndu = new double[n];
        double* _c = new double[n];


        for (int k = 0; k < n; ++k) {
            fscanf(f, "\n%c %lf %lf %lf", &xtype[k], &_bndl[k], &_bndu[k], &_c[k]);
        }
        bndl.setcontent(n, _bndl);
        bndu.setcontent(n, _bndu);

        c.setcontent(n, _c);
        delete[] _bndl;
        delete[] _bndu;
        delete[] _c;

        fscanf(f, "\n%d", &NNZ);
        double i, j, v;
        for (int k = 0; k < NNZ; ++k) {
            fscanf(f, "\n%lf %lf %lf", &i, &j, &v);
            sparseset(a, i, j, v);
        }
        
        double* _al = new double[m];
        double* _au = new double[m];
        for (int k = 0; k < m; ++k) {
            fscanf(f, "\n%lf %lf", &_al[k], &_au[k]);
        }
        al.setcontent(m, _al);
        au.setcontent(m, _au);
        delete[] _al;
        delete[] _au;
        fclose(f);

        minlpcreate(n, state);


        minlpsetcost(state, c);
        minlpsetbc(state, bndl, bndu);
        minlpsetlc2(state, a, al, au, m);


        // Solve
        minlpoptimize(state);
        minlpresults(state, x, rep);
        printf("%s\n", x.tostring(3).c_str());
        
    }
    catch (alglib::ap_error alglib_exception)
    {
        printf("ALGLIB exception with message '%s'\n", alglib_exception.msg.c_str());
        return 1;
    }
    return 0;
}