#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"

using namespace alglib;

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Rus");
    setlocale(LC_NUMERIC, "en_US");
    try
    {
        /*  
            sparsecreate()
            1. creation in a Hash-Table format
            2. insertion of the matrix elements
            3. conversion to the CRS representation
            4. matrix is passed to some linear algebra algorithm
            sparsecraetecrs()
            1. creation in a Hash-Table format
            2. insertion of the matrix elements
            3. conversion to the CRS representation
            4. matrix is passed to some linear algebra algorithm
        f(x1,x2) = 5x1 + 6x2 -> max
            3x1 + 5x2 ≤ 55
            x1 + 5x2 ≤ 45
            5x1 + 2x2 ≤ 60
            70x1 + 60x2 ≤ 1500
        */
        int n, m;
        sparsematrix a;
        real_2d_array out;
        real_1d_array al, au, x, c;
        integer_1d_array row_size = "[2, 2, 2, 2]";
        minlpstate state;
        minlpreport rep;

        read_csv("sparse_matrix.csv", ';', 0, out);

        n = out[0][0];
        m = out[0][1];
        sparsecreatecrs(n, m, row_size, a);
        //sparsecreate(n, m, a);
        ////заполняем sparseadd, sparseset
        //sparseconverttocrs(a);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                sparseset(a, i, j, out[0][i * m + j + 2]);

        al.setlength(n);
        for (int i = 0; i < n; ++i) al[i] = out[0][n * m + 2 + i];
        au.setlength(n);
        for (int i = 0; i < n; ++i) au[i] = out[0][n * m + 2 + n + i];
        c.setlength(m);
        for (int i = 0; i < m; ++i) c[i] = out[0][n * m + 2 + 2 * n + i];

        minlpcreate(m, state);

        minlpsetcost(state, c);
        minlpsetbcall(state, 0, 20);
        minlpsetlc2(state, a, al, au, n);


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