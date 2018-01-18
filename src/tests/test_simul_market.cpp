#include <string>
#include <iostream>
#include <sstream>
#include "../MonteCarlo.hpp"
#include "../BasketOption.hpp"
#include "../AsianOption.hpp"
#include "../PerformanceOption.hpp"
#include <cassert>

int main()
{
    // Initialisation
    bool print = true;
    double fdStep = 0, rho = 0.00,
           Basket_r = 0.04879,
           Basket_Strike = 100,
           Basket_Maturity = 3;
    int nbSamples = 50000,
        Basket_timestep_number = 1,
        Basket_option_size = 40;
    PnlVect *Basket_sigma = pnl_vect_create_from_scalar(Basket_option_size, 0.2),
            *Basket_spot = pnl_vect_create_from_scalar(Basket_option_size, 100),
            *Basket_weights = pnl_vect_create_from_scalar(Basket_option_size, 0.025),
            *trends = pnl_vect_create_from_scalar(Basket_option_size, Basket_r);

    BasketOption basket = BasketOption(Basket_Strike,
                                       Basket_Maturity,
                                       Basket_timestep_number,
                                       Basket_option_size,
                                       Basket_weights);
    BlackScholesModel bbsm = BlackScholesModel(Basket_option_size,
                                               Basket_r,
                                               rho,
                                               Basket_sigma,
                                               Basket_spot,
                                               trends);
    PnlMat *path = pnl_mat_create_from_zero(4, bbsm.size_);
    MonteCarlo bmc = MonteCarlo(fdStep, nbSamples, &basket, &bbsm);
    bbsm.simul_market(path,1, 3, bmc.rng_);
    assert(!pnl_mat_eq(path, pnl_mat_create_from_zero(4, bbsm.size_)));

    pnl_mat_print(path);
    pnl_vect_free(&Basket_weights);
    pnl_vect_free(&Basket_sigma);
    pnl_vect_free(&Basket_spot);
    pnl_vect_free(&trends);
    pnl_mat_free(&path);
    std::cout << "Test passed" << std::endl;
}
