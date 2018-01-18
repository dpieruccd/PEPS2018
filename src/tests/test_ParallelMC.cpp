#include "MonteCarlo.hpp"
#include "BasketOption.hpp"



int main()
{
    try
    {
        // Initialisation
        double fdStep = 0, rho = 0.00, prix, ic_50k, ic_75k,
               r = 0.04879, Strike = 100, Maturity = 3;

        int nbSamples_50k = 50000, nbSamples_75k = 75000,
            timestep_number = 1,
            option_size = 40;

        PnlVect *sigma = pnl_vect_create_from_scalar(option_size, 0.2);
        PnlVect *spot = pnl_vect_create_from_scalar(option_size, 100);
        PnlVect *weights = pnl_vect_create_from_scalar(option_size, 0.025);

        BasketOption basket = BasketOption(Strike,
                                           Maturity,
                                           timestep_number,
                                           option_size,
                                           weights);
        BlackScholesModel bbsm = BlackScholesModel(option_size,
                                                   r,
                          						   rho,
                           						   sigma,
                           						   spot);
        MonteCarlo bmc_50k = MonteCarlo(fdStep, nbSamples_50k, &basket, &bbsm);
    	bmc_50k.price(prix, ic_50k);
        pnl_vect_free(&sigma);
        pnl_vect_free(&spot);
        pnl_vect_free(&weights);
    }
    catch(const std::exception& e)
    {
	;
    }
    return 0;
}
