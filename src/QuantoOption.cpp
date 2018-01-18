#include "QuantoOption.hpp"
#include <iostream>
#include <cstdio>
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

double QuantoOption::payoff(const PnlMat *path){
  double sum_Quanto=MGET(path,nbTimeSteps_,0);
  if(sum_Quanto-K_>=0){
    return sum_Quanto-K_;
  }else{
    return 0.0;
  }
}
