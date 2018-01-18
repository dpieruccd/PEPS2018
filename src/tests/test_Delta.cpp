#include "../MonteCarlo.hpp"
#include "../AsianOption.hpp"
#include "../BasketOption.hpp"
#include "../Option.hpp"
#include "../Utilities.hpp"
#include <iostream>

using namespace std;

int main(){
  //1 delta per asset in t

  Utilities u;
  int Basket_timestep_number=1, Basket_option_size=40;
  double Basket_Maturity= 3,
  Basket_Strike = 100, Basket_r= 0.04879, rho = 0.0;

  PnlVect *Basket_sigma = pnl_vect_create_from_scalar(Basket_option_size, 0.2);
  PnlVect *Basket_spot = pnl_vect_create_from_scalar(Basket_option_size, 100);
  PnlVect *Basket_weights = pnl_vect_create_from_scalar(Basket_option_size, 0.025);



  Option *basket;
  basket = new BasketOption(Basket_Strike, Basket_Maturity, Basket_timestep_number, Basket_option_size, Basket_weights);



  BlackScholesModel bs(Basket_option_size, Basket_r, rho, Basket_sigma,Basket_spot);
  double fdStep = 0.1;
  int nbSamples = 500;


  MonteCarlo mc(fdStep,nbSamples, basket, &bs);

  PnlVect *deltas = pnl_vect_create_from_zero(bs.size_);
  double t = 0.2;

  PnlMat* p = pnl_mat_create_from_file("../data/simul_basket_1.dat");
  PnlMat* past = u.getConstatationDates(p, basket, 0);

  mc.delta(past,t,deltas);
  pnl_vect_print(deltas);

  pnl_vect_free(&Basket_spot);
  pnl_vect_free(&Basket_sigma);
  pnl_vect_free(&Basket_weights);
  pnl_vect_free(&deltas);
  pnl_mat_free(&past);
  delete basket;


}
