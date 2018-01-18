// test erreur de couverture
//
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include "../MonteCarlo.hpp"
#include "../AsianOption.hpp"


int main()
{
    // Initialisation
    int Asian_timestep_number = 150, Asian_option_size = 2, nbSamples = 1000;
    double  Asian_Maturity = 1.5, fdStep = 0.1, t = 0.2, H = 300,
            Asian_Strike = 100, Asian_r = 0.02, rho = 0.0,
            profitLoss;

    PnlVect *Asian_sigma = pnl_vect_create_from_scalar(Asian_option_size, 0.2),
            *Asian_spot = pnl_vect_create_from_scalar(Asian_option_size, 100),
            *Asian_weights = pnl_vect_create_from_scalar(Asian_option_size, 0.5);

    AsianOption asian = AsianOption(Asian_Strike,
                                    Asian_Maturity,
                                    Asian_timestep_number,
                                    Asian_option_size,
                                    Asian_weights);
    PnlMat *past = pnl_mat_create_from_file("../data/simul_asian.dat");
    BlackScholesModel bs = BlackScholesModel(Asian_option_size,
                                             Asian_r,
                                             rho,
                                             Asian_sigma,
                                             Asian_spot);

    MonteCarlo mc = MonteCarlo(fdStep, nbSamples, &asian, &bs);

    // Actual test
    mc.profitLoss(past, H, profitLoss);
    if (profitLoss  > 1 ){
      std::cerr << ("The P&L seems too high.") << std::endl;
    }
    std::cout << "profit loss:" << profitLoss << std::endl;

    // Free
    pnl_mat_free(&past);
    pnl_vect_free(&Asian_spot);
    pnl_vect_free(&Asian_sigma);
    pnl_vect_free(&Asian_weights);
    pnl_mat_free(&past);

    std::cout << "Test passed" << std::endl;
}
