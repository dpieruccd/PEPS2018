#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include "../MonteCarlo.hpp"
#include "../BasketOption.hpp"



int main()
{
    // Initialisation
    int Basket_timestep_number = 1, Basket_option_size = 40, nbSamples = 100;
    double Basket_Maturity = 3, fdStep = 0.1, t = 0.2, H = 1000,
           Basket_Strike = 100, Basket_r = 0.04879, rho = 0.0,
           profitLoss;

    PnlVect *Basket_sigma = pnl_vect_create_from_scalar(Basket_option_size, 0.2),
            *Basket_spot = pnl_vect_create_from_scalar(Basket_option_size, 100),
            *Basket_weights = pnl_vect_create_from_scalar(Basket_option_size, 0.025);

    BasketOption basket = BasketOption(Basket_Strike,
                                       Basket_Maturity,
                                       Basket_timestep_number,
                                       Basket_option_size,
                                       Basket_weights);

    PnlMat *past = pnl_mat_create_from_file("../data/simul_basket_1.dat");

    BlackScholesModel bs = BlackScholesModel(Basket_option_size,
                                             Basket_r,
                                             rho,
                                             Basket_sigma,
                                             Basket_spot);
    MonteCarlo mc = MonteCarlo(fdStep,nbSamples, &basket, &bs);

    // Actual test
    mc.profitLoss(past, H, profitLoss);
    if (profitLoss  > 1 ){
      std::cerr << ("The P&L seems too high.") << std::endl;
    }

    std::cout << "profit loss:" << profitLoss << std::endl;

    // Free
    pnl_mat_free(&past);
    pnl_vect_free(&Basket_spot);
    pnl_vect_free(&Basket_sigma);
    pnl_vect_free(&Basket_weights);
    pnl_mat_free(&past);
    std::cout << "Test passed" << std::endl;
}
