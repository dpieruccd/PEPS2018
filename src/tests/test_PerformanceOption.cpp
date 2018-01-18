// Asian test
//
#include <cassert>
#include <iostream>
#include <string>
#include "../PerformanceOption.hpp"

//Check that we can instanciate an PerformanceOption and compute its payoff
int main()
{
    PnlVect *weigths = pnl_vect_create_from_scalar(3, 0.2);
    PerformanceOption a = PerformanceOption(10, 5, 3, weigths);
    PnlMat *path = pnl_mat_create_from_scalar(a.nbTimeSteps_+1, a.size_,20);

    assert(a.payoff(path)==1);

    pnl_vect_free(&weigths);
    pnl_mat_free(&path);

    std::cout << "Test passed" << std::endl;
}
