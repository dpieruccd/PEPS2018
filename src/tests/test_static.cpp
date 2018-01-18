// Static test
//
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include "MonteCarlo.hpp"
#include "BasketOption.hpp"
#include "AsianOption.hpp"
#include "PerformanceOption.hpp"

int main()
{
    try
    {
        // Initialisation
        bool print = true;
        double fdStep = 0, rho = 0.00, Perf_rho = 0.5,
            Basket_r = 0.04879, Asian_r = 0.02, Perf_r = 0.03,
            Basket_Strike = 100, Asian_Strike = 100,
            Basket_Maturity = 3, Asian_Maturity = 1.5, Perf_Maturity = 2;
        int nbSamples = 50000,
            Basket_timestep_number = 1,  Asian_timestep_number = 150, Perf_timestep_number = 12,
            Basket_option_size = 40, Asian_option_size = 2,   Perf_option_size = 5;

            // Asian vectors
            PnlVect *Asian_sigma = pnl_vect_create_from_scalar(Asian_option_size, 0.2);
            PnlVect *Asian_spot = pnl_vect_create_from_scalar(Asian_option_size, 100);
            PnlVect *Asian_weights = pnl_vect_create_from_scalar(Asian_option_size, 0.5);

            AsianOption asian = AsianOption(Asian_Strike,
                                            Asian_Maturity,
                                            Asian_timestep_number,
                                            Asian_option_size,
                                            Asian_weights);

            BlackScholesModel absm = BlackScholesModel(Asian_option_size,
                            						   Asian_r,
                            						   rho,
                            						   Asian_sigma,
                            						   Asian_spot);

            PnlRng* cstGen = pnl_rng_create(1);
            pnl_rng_sseed(cstGen, time(NULL));

            PnlMat *path = pnl_mat_create_from_zero(asian.nbTimeSteps_+1, absm.size_);
            absm.asset(path, asian.T_, asian.nbTimeSteps_, cstGen);
            pnl_mat_print(path);

            pnl_vect_free(&Asian_spot);
            pnl_vect_free(&Asian_sigma);
            pnl_vect_free(&Asian_weights);
            pnl_rng_free(&cstGen);
            pnl_mat_free(&path);
    }
    catch(const std::exception &e)
    {
        std::cerr << "Test failed" << std::endl;
    }

    return 0;
}
