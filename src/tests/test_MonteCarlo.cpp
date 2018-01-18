// MonteCarlo test
//
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include "../MonteCarlo.hpp"
#include "../BasketOption.hpp"
#include "../AsianOption.hpp"
#include "../PerformanceOption.hpp"

void runTestIncrement1(MonteCarlo& mc,
                       double& prix, double& ic,
                       std::stringstream& ss)
{
    ss  << "----" << mc.nbSamples_ << std::endl << "        "
	<< "Update term:" << mc.actualisation(0.0) << std::endl << "        ";
    mc.price(prix, ic);
    ss  << "Price in t0:" << prix << std::endl << "        "
        << "Confidence interval: [" << prix-ic << ", "
                      		    << prix+ic << "]" << std::endl;
}

void runTestIncrement2(MonteCarlo& mc,
                       double& prix, double& ic,
                       std::stringstream& ss)
{
    double t = 1 ;
    // Initialisation for incr 2
    int nbStep = -floor(-t * mc.opt_->nbTimeSteps_ / mc.opt_->T_);
    PnlMat *pastPath = pnl_mat_create_from_zero(nbStep+1, mc.mod_->size_);
    mc.mod_->asset(pastPath, t, nbStep, mc.rng_);

    // Result
    ss  << "----" << mc.nbSamples_ << std::endl << "        "
  	    << "Update term:" << mc.actualisation(t) << std::endl << "        ";
  	mc.price(pastPath, t, prix, ic);
    ss  << "t1:" << t << std::endl << "        "
        << "nbStep:" << nbStep << std::endl << "        "
        << "Price:" << prix << std::endl << "        "
  	    << "Confidence interval: [" << prix-ic << ", "
                      		        << prix+ic << "]" << std::endl;
}

int main()
{
    // Initialisation
    bool print = false; // set to false

    double fdStep = 0, rho = 0.00, Perf_rho = 0.5, prix, ic_50k, ic_75k,
           Basket_r = 0.04879, Asian_r = 0.02, Perf_r = 0.03,
           Basket_Strike = 100, Asian_Strike = 100,
           Basket_Maturity = 3, Asian_Maturity = 1.5, Perf_Maturity = 2;

    int nbSamples_50k = 50000, nbSamples_75k = 75000,
        Basket_timestep_number = 1,  Asian_timestep_number = 150, Perf_timestep_number = 12,
        Basket_option_size = 40, Asian_option_size = 2,   Perf_option_size = 5;

    std::stringstream ss;

    // Basket vectors
    PnlVect *Basket_sigma = pnl_vect_create_from_scalar(Basket_option_size, 0.2),
            *Basket_spot = pnl_vect_create_from_scalar(Basket_option_size, 100),
            *Basket_weights = pnl_vect_create_from_scalar(Basket_option_size, 0.025),

    // Asian vectors
            *Asian_sigma = pnl_vect_create_from_scalar(Asian_option_size, 0.2),
            *Asian_spot = pnl_vect_create_from_scalar(Asian_option_size, 100),
            *Asian_weights = pnl_vect_create_from_scalar(Asian_option_size, 0.5),

    // Performance vectors
            *Perf_sigma = pnl_vect_create_from_scalar(Perf_option_size, 0.2),
            *Perf_spot = pnl_vect_create_from_scalar(Perf_option_size, 100),
            *Perf_weights = pnl_vect_create_from_scalar(Perf_option_size, 0.2);

    // Initialisation Option
    BasketOption basket = BasketOption(Basket_Strike,
                                       Basket_Maturity,
                                       Basket_timestep_number,
                                       Basket_option_size,
                                       Basket_weights);
    AsianOption asian = AsianOption(Asian_Strike,
                                    Asian_Maturity,
                                    Asian_timestep_number,
                                    Asian_option_size,
                                    Asian_weights);
    PerformanceOption performance = PerformanceOption(Perf_Maturity,
                                                      Perf_timestep_number,
                                                      Perf_option_size,
                                                      Perf_weights);

    // Initialisation BlackScholesModel
    BlackScholesModel bbsm = BlackScholesModel(Basket_option_size,
                                               Basket_r,
                                               rho,
                        		       Basket_sigma,
                        		       Basket_spot),
       		           absm = BlackScholesModel(Asian_option_size,
                                               Asian_r,
                                               rho,
                                               Asian_sigma,
                                               Asian_spot),
                      pbsm = BlackScholesModel(Perf_option_size,
                                               Perf_r,
                                               Perf_rho,
                                               Perf_sigma,
                                               Perf_spot);

    // Initialisation MonteCarlo
    MonteCarlo bmc_50k = MonteCarlo(fdStep, nbSamples_50k, &basket, &bbsm),
               amc_50k = MonteCarlo(fdStep, nbSamples_50k, &asian, &absm),
               pmc_50k = MonteCarlo(fdStep, nbSamples_50k, &performance, &pbsm),
               bmc_75k = MonteCarlo(fdStep, nbSamples_75k, &basket, &bbsm),
               amc_75k = MonteCarlo(fdStep, nbSamples_75k, &asian, &absm),
               pmc_75k = MonteCarlo(fdStep, nbSamples_75k, &performance, &pbsm);

    // Tests
    ss  << "Basket option" << std::endl
        << "--Increment 1" << std::endl;
    runTestIncrement1(bmc_50k, prix, ic_50k, ss);
    runTestIncrement1(bmc_75k, prix, ic_75k, ss);
    assert(ic_50k > ic_75k);
    ss  << "--Increment 2" << std::endl;
    runTestIncrement2(bmc_50k, prix, ic_50k, ss);
    runTestIncrement2(bmc_75k, prix, ic_75k, ss);
    assert(ic_50k > ic_75k);

    ss  << "==================" << std::endl
        << "Asian option" << std::endl
        << "--Increment 1" << std::endl;
    runTestIncrement1(amc_50k, prix, ic_50k, ss);
    runTestIncrement1(amc_75k, prix, ic_75k, ss);
    assert(ic_50k > ic_75k);
    ss  << "--Increment 2" << std::endl;
    runTestIncrement2(amc_50k, prix, ic_50k, ss);
    runTestIncrement2(amc_75k, prix, ic_75k, ss);
    assert(ic_50k > ic_75k);

    ss  << "==================" << std::endl
        << "Performance option" << std::endl
        << "--Increment 1" << std::endl;
    runTestIncrement1(pmc_50k, prix, ic_50k, ss);
    runTestIncrement1(pmc_75k, prix, ic_75k, ss);
    assert(ic_50k > ic_75k);
    ss  << "--Increment 2" << std::endl;
    runTestIncrement2(pmc_50k, prix, ic_50k, ss);
    runTestIncrement2(pmc_75k, prix, ic_75k, ss);
    assert(ic_50k > ic_75k);

    if(print)
        std::cout << ss.str();

    pnl_vect_free(&Basket_sigma);
    pnl_vect_free(&Basket_spot);
    pnl_vect_free(&Basket_weights);
    pnl_vect_free(&Asian_sigma);
    pnl_vect_free(&Asian_spot);
    pnl_vect_free(&Asian_weights);
    pnl_vect_free(&Perf_sigma);
    pnl_vect_free(&Perf_spot);
    pnl_vect_free(&Perf_weights);

    std::cout << "Test passed" << std::endl;
}
