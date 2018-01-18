#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include "../MonteCarlo.hpp"
#include "../BasketOption.hpp"


int main()
{
    int Call_timestep_number = 1, Call_option_size = 1, nbSamples = 1000;
    double Call_Maturity = 1, fdStep = 0.1, t = 0.2, H = 365,
           Call_Strike = 100, Call_r = 0.04879, rho = 0.0,
           profitLoss;

    PnlVect *Call_sigma = pnl_vect_create_from_scalar(Call_option_size, 0.2),
            *Call_spot = pnl_vect_create_from_scalar(Call_option_size, 100),
            *Call_weights = pnl_vect_create_from_scalar(Call_option_size, 1);

    BasketOption basket = BasketOption(Call_Strike,
                                       Call_Maturity,
                                       Call_timestep_number,
                                       Call_option_size,
                                       Call_weights);
    PnlMat *past = pnl_mat_create_from_file("../data/simul_call.dat");
    BlackScholesModel bs = BlackScholesModel(Call_option_size,
                                             Call_r,
                                             rho,
                                             Call_sigma,
                                             Call_spot);

    MonteCarlo mc = MonteCarlo(fdStep,nbSamples, &basket, &bs);

    // Actual test
    mc.profitLoss(past, H, profitLoss);
    if (profitLoss  > 1 ){
      std::cerr << ("The P&L seems too high.") << std::endl;
    }

    std::cout << "profit loss:" << profitLoss << std::endl;

    pnl_mat_free(&past);
    pnl_vect_free(&Call_spot);
    pnl_vect_free(&Call_sigma);
    pnl_vect_free(&Call_weights);
    pnl_mat_free(&past);
    std::cout << "Test passed" << std::endl;
}
