#include <string>
#include <iostream>
#include "../BlackScholesModel.hpp"
#include <cassert>

//Check that asset and shift_asset are working correctly
int main()
{
    // Initialisation,
    PnlVect *sigma = pnl_vect_create_from_scalar(5, 0.3);
    PnlVect *spot = pnl_vect_create_from_scalar(5, 10);
    BlackScholesModel b(5,0.01,0.2,sigma,spot);
    assert(b.size_==5);
    assert(b.r_ == 0.01);
    assert(b.rho_ == 0.2);
    assert(pnl_vect_eq(sigma, b.sigma_));
    assert(pnl_vect_eq(spot, b.spot_));
    assert(b.corr != NULL);
    assert(b.g != NULL);

    PnlMat *path = pnl_mat_create_from_zero(11, 5);
    double T = 50;
    int stepNumber = 10;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    b.asset(path, T, stepNumber, rng);
    assert (! pnl_mat_eq(path, pnl_mat_create_from_zero(11, 5)));

    PnlMat *shift_path=pnl_mat_create_from_zero(11,5);
    b.shiftAsset(shift_path,path,1,0.5,2,T/stepNumber);
    int dep = floor(2*stepNumber/T)+1;
    for(int i=dep;i<path->m;i++){
      assert (pnl_mat_get(shift_path, i, 1) == (1+0.5)*pnl_mat_get(path,i,1));
    }

    // Liberation
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_mat_free(&path);
    pnl_mat_free(&shift_path);
    pnl_rng_free(&rng);

    std::cout << "Test passed" << std::endl;
}
