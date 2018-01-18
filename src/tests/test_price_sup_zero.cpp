#include "../MonteCarlo.hpp"
#include "../BasketOption.hpp"
#include "../BlackScholesModel.hpp"
#include <iostream>
#include <cassert>

using namespace std;

int main(){
  std::cout << "Test of the basket at t>0" << std::endl;

  //Computation at t = 0.03
  double fdStep = 0.1;
  int nbSamples = 500000;
  double t = 1;

  int Basket_timestep_number=1, Basket_option_size=5;
  double Basket_Maturity= 3,
  Basket_Strike = 100, Basket_r= 0.04879, rho = 0.0;

  PnlVect *Basket_sigma = pnl_vect_create_from_scalar(Basket_option_size, 0.2);
  PnlVect *Basket_spot = pnl_vect_create_from_scalar(Basket_option_size, 100);
  PnlVect *Basket_weights = pnl_vect_create_from_scalar(Basket_option_size, 0.2);

  PnlMat *Past =  pnl_mat_create_from_scalar(2,Basket_option_size, 100);
  PnlVect *SpotT = pnl_vect_create(Basket_option_size);
  pnl_vect_set(SpotT, 0, 98.551650 );
  pnl_vect_set(SpotT, 1, 104.246846 );
  pnl_vect_set(SpotT, 2, 105.978338 );
  pnl_vect_set(SpotT, 3, 101.410858 );
  pnl_vect_set(SpotT, 4, 100.656438);
  pnl_mat_set_row(Past, SpotT, 1);

  BlackScholesModel bs1(Basket_option_size, Basket_r, rho, Basket_sigma, Basket_spot);
  BasketOption basket1(Basket_Strike, Basket_Maturity, Basket_timestep_number, Basket_option_size, Basket_weights);

  MonteCarlo mc1(fdStep, nbSamples, &basket1, &bs1);
  double price1, conf1;
  mc1.price(Past, t, price1, conf1);
  double inf1 = price1 - conf1;
  double max1 = price1 + conf1;

  //Computation at t=0
  BlackScholesModel bs2(Basket_option_size, Basket_r, rho, Basket_sigma, SpotT);
  BasketOption basket2(Basket_Strike, Basket_Maturity - t, Basket_timestep_number, Basket_option_size, Basket_weights);
  MonteCarlo mc2(fdStep, nbSamples, &basket2, &bs2);
  double price2, conf2;
  mc2.price(price2, conf2);
  double inf2 = price2 - conf2;
  double max2 = price2 + conf2;

  assert(!((max1 < inf2)||(max2 < inf1)));
  std::cout << "Test passed" << std::endl;

}
