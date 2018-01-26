#include "PerformanceOption.hpp"


PerformanceOption::PerformanceOption(double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1)
  :Option(T1,nbTimeSteps1,size1,lambdas1)
{}
PerformanceOption::PerformanceOption()
  :Option()
{}

PerformanceOption::~PerformanceOption()
{}

double PerformanceOption::exactPrice0(BlackScholesModel *b){
    return 0.0;
}
double PerformanceOption::payoff(const PnlMat *path){
  double sum_perf1=0;// somme sur les actifs(numerateur)
  double sum_perf2=0;// somme sur les actifs(denominateur)
  double sum_perf3=0;// somme sur les pas de temps
  double quotient;
  int i,j;
  for(i=1;i<=nbTimeSteps_;i++){
    for(j=0;j<size_;j++){

      sum_perf1=sum_perf1+pnl_vect_get(lambdas_,j)*MGET(path,i,j);
    }
    for(j=0;j<size_;j++){

      sum_perf2=sum_perf2+pnl_vect_get(lambdas_,j)*MGET(path,i-1,j);
    }
    quotient=sum_perf1/sum_perf2;
  	sum_perf1=0;
  	sum_perf2=0;
    if(quotient-1>0)
      {
	       sum_perf3=sum_perf3+quotient-1;
      }
  }

  return 1+sum_perf3;
}
