
#include "../BasketOption.hpp"
#include "../AsianOption.hpp"
#include "../PerformanceOption.hpp"
#include <iostream>

using namespace std;

int main(){

    //Test BasketOption
    PnlVect *weigths=pnl_vect_create_from_scalar(3,0.2);
    BasketOption b=BasketOption(20.0, 10,5,3,weigths);
    PnlMat *path1=pnl_mat_create_from_scalar(b.nbTimeSteps_+1,b.size_,20);

    cout<<b.payoff(path1)<<endl;

    //Test AsianOption
    AsianOption a=AsianOption(20, 10,5,3,weigths);
    PnlMat *path2=pnl_mat_create_from_scalar(a.nbTimeSteps_+1,a.size_,20);

    cout<<a.payoff(path2)<<endl;

    //Test PerformanceOption
    PerformanceOption p=PerformanceOption(10,5,3,weigths);
    PnlMat *path3=pnl_mat_create_from_scalar(p.nbTimeSteps_+1,p.size_,20);

    cout<<p.payoff(path3)<<endl;
    pnl_vect_free(&weigths);
    pnl_mat_free(&path1);
    pnl_mat_free(&path2);
    pnl_mat_free(&path3);

    return 0;
}
