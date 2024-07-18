#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"

using namespace alglib;

void answer(minlpstate& state, minlpreport& rep, real_1d_array& x, real_1d_array& c, real_1d_array a, const double al, const double au) {
    minlpaddlc2dense(state, a, al, au);
    minlpsetalgoipm(state);
    minlpoptimize(state);
    minlpresults(state, x, rep);
    printf("%s\n", x.tostring(3).c_str());
    printf("%f\n\n", -c[0] * x[0] - c[1] * x[1]);
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Rus");
    setlocale(LC_NUMERIC, "en_US");
    try
    {
        /*  f(x1,x2) = 5x1 + 6x2 -> max
            3x1 + 5x2 ≤ 55
            x1 + 5x2 ≤ 45
            5x1 + 2x2 ≤ 60
            70x1 + 60x2 ≤ 1500
        */
        real_1d_array c = "[-5, -6]";
        real_1d_array x;
        minlpstate state;
        minlpreport rep;

        minlpcreate(2, state);

        minlpsetcost(state, c);
        minlpsetbcall(state, 0, 100);

        printf("Одно условие\n");
        answer(state, rep, x, c, "[3, 5]", fp_neginf, 55);
        printf("Второе условие\n");
        answer(state, rep, x, c, "[1, 5]", fp_neginf, 45);
        printf("Третье условие\n");     
        answer(state, rep, x, c, "[5, 2]", fp_neginf, 60);
        printf("Четвертое условие\n");
        answer(state, rep, x, c, "[70, 60]", fp_neginf, 1500);
        printf("Пятое условие\n");
        answer(state, rep, x, c, "[1, 1]", 5, 10);

    }
    catch (alglib::ap_error alglib_exception)
    {
        printf("ALGLIB exception with message '%s'\n", alglib_exception.msg.c_str());
        return 1;
    }
    return 0;
}