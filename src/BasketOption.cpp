#include "BasketOption.hpp"


BasketOption::BasketOption(double strike, double T1,int nbTimeSteps1,int size1, PnlVect *lambdas1)
  :Option(T1, nbTimeSteps1, size1, lambdas1)
{
  K_ = strike;
}

BasketOption::BasketOption()
  :Option()
{}


BasketOption::~BasketOption()
{}

double BasketOption::payoff(const PnlMat *path){
  double sum_basket=0;
  int i;
  for(i=0;i<size_;i++){
    sum_basket+=pnl_vect_get(lambdas_,i)*MGET(path,nbTimeSteps_,i);
  }

  if(sum_basket-K_>=0){
    return sum_basket-K_;
  }else{
    return 0;
  }
}
