#include "QuantoOption.hpp"
#include <iostream>
#include <cstdio>
#include "pnl/pnl_finance.h"
using namespace std;

QuantoOption::QuantoOption(double strike, double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1)
  :Option(T1, nbTimeSteps1, size1,lambdas1)
{
  K_ = strike;
}

QuantoOption::QuantoOption()
  :Option()
{}


QuantoOption::~QuantoOption()
{}
double QuantoOption::exactPrice0(BlackScholesModel *b){
    double rf= b->r_-pnl_vect_get(b->trends_,1);
    return pnl_bs_call(pnl_vect_get(b->spot_,0)*exp(T_*(-b->r_ + rf - b->rho_*pnl_vect_get(b->sigma_,0)*pnl_vect_get(b->sigma_,0))),K_,T_,b->r_,0,pnl_vect_get(b->sigma_,0));
}
double QuantoOption::payoff(const PnlMat *path){
  double sum_Quanto=MGET(path,nbTimeSteps_,0);
  if(sum_Quanto-K_>=0){
    return sum_Quanto-K_;
  }else{
    return 0.0;
  }
}
