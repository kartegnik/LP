#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"

using namespace alglib;

int main(int argc, char** argv)
{
    try
    {
        //
        // This example demonstrates how to minimize
        //
        //     F(x0,x1) = -0.1*x0 - x1
        //
        // subject to box constraints
        //
        //     -1 <= x0,x1 <= +1 
        //
        // and general linear constraints
        //
        //     x0 - x1 >= -1
        //     x0 + x1 <=  1
        //
        // We use dual simplex solver provided by ALGLIB for this task. Box
        // constraints are specified by means of constraint vectors bndl and
        // bndu (we have bndl<=x<=bndu). General linear constraints are
        // specified as AL<=A*x<=AU, with AL/AU being 2x1 vectors and A being
        // 2x2 matrix.
        //
        // NOTE: some/all components of AL/AU can be +-INF, same applies to
        //       bndl/bndu. You can also have AL[I]=AU[i] (as well as
        //       BndL[i]=BndU[i]).
        // 
        /*  f(x1,x2) = 5x1 + 6x2 -> max
            3x1 + 5x2 ≤ 55
            x1 + 5x2 ≤ 45
            5x1 + 2x2 ≤ 60 
            70x1 + 60x2 ≤ 1500
        */
        real_2d_array a = "[[3,5],[1,5],[5,2],[70,60]]";
        real_1d_array al = "[-inf,-inf,-inf,-inf]";
        real_1d_array au = "[55,45,62,1500]";
        real_1d_array c = "[-5, -6]";
        real_1d_array s = "[1,1,1,1]";
        real_1d_array bndl = "[0,0]";
        real_1d_array bndu = "[+20,+20]";
        real_1d_array x;
        minlpstate state;
        minlpreport rep;

        minlpcreate(2, state);

        //
        // Set cost vector, box constraints, general linear constraints.
        //
        // Box constraints can be set in one call to minlpsetbc() or minlpsetbcall()
        // (latter sets same constraints for all variables and accepts two scalars
        // instead of two vectors).
        //
        // General linear constraints can be specified in several ways:
        // * minlpsetlc2dense() - accepts dense 2D array as input; sometimes this
        //   approach is more convenient, although less memory-efficient.
        // * minlpsetlc2() - accepts sparse matrix as input
        // * minlpaddlc2dense() - appends one row to the current set of constraints;
        //   row being appended is specified as dense vector
        // * minlpaddlc2() - appends one row to the current set of constraints;
        //   row being appended is specified as sparse set of elements
        // Independently from specific function being used, LP solver uses sparse
        // storage format for internal representation of constraints.
        //
        minlpsetcost(state, c);
        minlpsetbc(state, bndl, bndu);
        minlpsetlc2dense(state, a, al, au, 4);
        //
        // Set scale of the parameters.
        //
        // It is strongly recommended that you set scale of your variables.
        // Knowing their scales is essential for evaluation of stopping criteria
        // and for preconditioning of the algorithm steps.
        // You can find more information on scaling at http://www.alglib.net/optimization/scaling.php
        //
        minlpsetscale(state, s);

        // Solve
        minlpoptimize(state);
        minlpresults(state, x, rep);
        printf("%s\n", x.tostring(3).c_str()); // EXPECTED: [0,1]
    }
    catch (alglib::ap_error alglib_exception)
    {
        printf("ALGLIB exception with message '%s'\n", alglib_exception.msg.c_str());
        return 1;
    }
    return 0;
}
