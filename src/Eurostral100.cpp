#include "Eurostral100.hpp"
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

Eurostral100::Eurostral100(double strike, double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1)
  :Option(T1, nbTimeSteps1, size1,lambdas1)
{
  K_ = strike;
}

Eurostral100::Eurostral100()
  :Option()
{}


Eurostral100::~Eurostral100()
{}

double Eurostral100::payoff(const PnlMat *path){
  PnlVect*  indice1 = pnl_vect_create(path->m);
  PnlVect* indice2 = pnl_vect_create(path->m);
  PnlVect* indice3 = pnl_vect_create(path->m);
  pnl_mat_get_col(indice1,path,0);
  pnl_mat_get_col(indice2,path,1);
  pnl_mat_get_col(indice3,path,2);
  double payoff,init1,init2,init3,cour1,cour2,cour3,ma,mi,entre;
  init1 = pnl_vect_get(indice1,0);
  init2 = pnl_vect_get(indice2,1);
  init3 = pnl_vect_get(indice3,2);
  for ( int i = 0 ; i < 16 ; i++ ) {
    cour1 = (pnl_vect_get(indice1,i+1)-init1)/init1;
    cour2 = (pnl_vect_get(indice2,i+1)-init2)/init2;
    cour3 = (pnl_vect_get(indice3,i+1)-init3)/init3;
    
    ma = max(max(cour1,cour2),cour3);
    mi = min(min(cour1,cour2),cour3);
    if ( cour1 != ma && cour1 != mi ) {
      entre = cour1;
    } else if ( cour2 != ma && cour2 != mi ) {
      entre = cour2;
    } else {
      entre = cour3;
    }
    payoff += 0.5*ma + 0.3*entre + 0.2*mi;
  }
  payoff /= 16 ;
  if ( payoff < 0 ) {
    return 1.0;
  }
  return 1.0+0.55*payoff;
}


